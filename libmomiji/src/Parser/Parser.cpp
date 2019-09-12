#include <momiji/Parser.h>
#include <momiji/Types.h>

#include "./Utils.h"
#include "Combinators.h"
#include "Common.h"
#include "Mappings.h"
#include <momiji/Utils.h>
#include <optional>

namespace momiji
{
    namespace
    {
        auto make_parser_error(std::int64_t column,
                               std::int64_t line,
                               const ParserError::ErrorType& error,
                               std::string_view sourceCode)
        {
            return nonstd::make_unexpected<ParserError>(
                { line, column, std::move(error), std::string { sourceCode } });
        }

        constexpr bool isInternal(momiji::InstructionType instr) noexcept
        {
            return instr == InstructionType::Breakpoint ||
                   instr == InstructionType::HaltCatchFire;
        }

        constexpr bool isDirective(momiji::InstructionType instr) noexcept
        {
            return instr == InstructionType::DataMarker ||
                   instr == InstructionType::CodeMarker;
        }

        constexpr bool isBranchInstr(momiji::InstructionType instr) noexcept
        {
            return (instr == InstructionType::Branch) ||
                   (instr == InstructionType::BranchCondition) ||
                   (instr == InstructionType::BranchSubroutine);
        }

        constexpr bool isSection(momiji::InstructionType instr) noexcept
        {
            return instr == InstructionType::CodeMarker ||
                   instr == InstructionType::DataMarker;
        }

        constexpr std::optional<ParserSection>
        convSection(momiji::InstructionType instr) noexcept
        {
            switch (instr)
            {
            case InstructionType::DataMarker:
                return ParserSection::Data;

            case InstructionType::CodeMarker:
                return ParserSection::Code;

            default:
                return std::nullopt;
            }
        }

        constexpr bool checkSections(ParserSection sectionType,
                                     InstructionType instr) noexcept
        {
            if (isSection(instr))
            {
                return true;
            }

            if (sectionType == ParserSection::Code &&
                instr == InstructionType::Declare)
            {
                return false;
            }

            if (sectionType == ParserSection::Data &&
                instr != InstructionType::Declare)
            {
                return false;
            }

            return true;
        }

        constexpr int requiresImmediateData(const momiji::Operand& op,
                                            momiji::DataType dataType) noexcept
        {
            if (std::holds_alternative<momiji::operands::Immediate>(op))
            {
                switch (dataType)
                {
                case DataType::Byte:
                case DataType::Word:
                    return 2;

                case DataType::Long:
                    return 4;
                }
            }

            if (std::holds_alternative<momiji::operands::AbsoluteShort>(op))
            {
                return 2;
            }

            if (std::holds_alternative<momiji::operands::AbsoluteLong>(op))
            {
                return 4;
            }

            if (std::holds_alternative<momiji::operands::AddressOffset>(op) ||
                std::holds_alternative<momiji::operands::AddressIndex>(op))
            {
                return 2;
            }

            return 0;
        }

        std::int32_t visitNum(const momiji::objects::Number& num);
        std::int32_t visitLabel(const momiji::objects::Label& label);
        std::int32_t visitOp(const momiji::objects::MathOperator& operand);

        std::int32_t visitNum(const momiji::objects::Number& num,
                              const momiji::LabelInfo&)
        {
            return num.number;
        }

        std::int32_t visitLabel(const momiji::objects::Label& currlabel,
                                const momiji::LabelInfo& labels)
        {
            auto res = std::find_if(std::begin(labels),
                                    std::end(labels),
                                    [&](const momiji::Label& label) {
                                        return label.nameHash == currlabel.hash;
                                    });

            if (res == std::end(labels))
            {
                return 0;
            }

            return std::int32_t(res->idx);
        }

        std::int32_t visitOp(const momiji::objects::MathOperator& operand,
                             const momiji::LabelInfo& labels)
        {
            using namespace momiji::objects;

            const auto& left  = *operand.left;
            const auto& right = *operand.right;

            switch (operand.type)
            {
            case MathOperator::Type::Add:
                return resolveAST(left, labels) + resolveAST(right, labels);

            case MathOperator::Type::Sub:
                return resolveAST(left, labels) - resolveAST(right, labels);

            case MathOperator::Type::Mul:
                return resolveAST(left, labels) * resolveAST(right, labels);

            case MathOperator::Type::Div:
                return resolveAST(left, labels) / resolveAST(right, labels);
            }

            return 1;
        }

        bool checkLabelsInAST(const momiji::objects::MathASTNode& node,
                              const momiji::LabelInfo& labels)
        {
            if (std::holds_alternative<objects::Label>(node.value))
            {
                auto& label = std::get<objects::Label>(node.value);
                auto res =
                    std::find_if(std::begin(labels),
                                 std::end(labels),
                                 [&](const momiji::Label& foundLabel) {
                                     return label.hash == foundLabel.nameHash;
                                 });

                if (res == std::end(labels))
                {
                    return false;
                }

                return true;
            }

            if (std::holds_alternative<objects::MathOperator>(node.value))
            {
                auto& op = std::get<objects::MathOperator>(node.value);
                return checkLabelsInAST(*op.left, labels) &&
                       checkLabelsInAST(*op.right, labels);
            }

            return true;
        }
    } // namespace

    struct Parser
    {
        Parser(std::string_view str)
            : str(str)
            , settings { ParserSettings {} }
        {
        }

        Parser(std::string_view str, ParserSettings settings)
            : str(str)
            , settings(settings)
        {
        }

        momiji::ParsingResult run()
        {
            auto tmp_str = str;
            while (!tmp_str.empty() && !parsing_error)
            {
                momiji::ParsedInstruction instr;

                {
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str              = skip_whitespace.rest;
                }

                auto skip_comments = Comment()(tmp_str);
                if (skip_comments.result)
                {
                    tmp_str = skip_comments.rest;
                    ++line_count;
                    continue;
                }

                auto try_label = ParseLabel()(tmp_str);
                if (try_label.result)
                {
                    // We found a label!
                    auto label_hash = utils::hash(try_label.parsed_str);
                    labels.emplace_back(
                        label_hash, program_counter, try_label.parsed_str);

                    tmp_str              = try_label.rest;
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str              = skip_whitespace.rest;
                }

                // This is a decent time to check for breakpoints
                if (!settings.breakpoints.empty())
                {
                    handleBreakpoints();
                }

                // Maybe we have an instruction?
                auto instrword = InstructionWord()(tmp_str);
                tmp_str        = instrword.rest;

                // If we don't find an instruction, we just skip the
                // line
                if (!instrword.result)
                {
                    auto skip_to_end = SkipLine()(tmp_str);
                    tmp_str          = skip_to_end.rest;
                    ++line_count;
                    continue;
                }

                const auto str_hash = momiji::utils::hash(instrword.parsed_str);

                // Check for a matching instruction
                auto found_instr =
                    std::find_if(std::begin(momiji::mappings),
                                 std::end(momiji::mappings),
                                 [str_hash](momiji::MappingType hash) {
                                     return hash.mapping == str_hash;
                                 });

                if (found_instr == std::end(momiji::mappings))
                {
                    auto error        = errors::NoInstructionFound {};
                    error.inputString = instrword.parsed_str;

                    auto begin = instrword.parsed_str.data();
                    auto end   = SkipLine()(instrword.rest).rest.data();

                    auto diff = end - begin;

                    return make_parser_error(
                        0,
                        line_count,
                        error,
                        std::string_view { begin, std::uint32_t(diff) });
                }

                auto res = found_instr->execfn(tmp_str, instr);

                if (res.result)
                {
                    auto skip_line = SkipLine()(res.rest);

                    const auto begin = instrword.parsed_str.data();
                    const auto end   = skip_line.rest.data();

                    instr.programCounter = std::int32_t(program_counter);

                    instr.sourceLine = std::int32_t(line_count);

                    if (isSection(instr.instructionType))
                    {
                        currentSection = convSection(instr.instructionType);
                    }

                    if (currentSection &&
                        !checkSections(*currentSection, instr.instructionType))
                    {
                        errors::UnexpectedSectionContent error;
                        error.section = *currentSection;
                        return make_parser_error(
                            0,
                            line_count,
                            error,
                            std::string_view(begin, std::size_t(end - begin)));
                    }

                    handlePCIncrement(instr);

                    instructionStr.emplace_back(begin,
                                                std::size_t(end - begin));

                    instructions.emplace_back(std::move(instr));
                }
                else
                {
                    auto skip_line = SkipLine()(res.rest);

                    const auto begin = instrword.parsed_str.data();
                    const auto end   = skip_line.rest.data();

                    return make_parser_error(
                        0,
                        line_count,
                        res.error,
                        std::string_view(begin, std::uint32_t(end - begin)));
                }

                tmp_str = res.rest;

                {
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str              = skip_whitespace.rest;
                    auto detect_comment  = Comment()(tmp_str);
                    tmp_str              = detect_comment.rest;

                    // Maybe we have a comment?
                    if (detect_comment.result)
                    {
                        // If so skip the line
                        ++line_count;
                        continue;
                    }

                    // Else we may have a character?
                    auto detect_any_char = NotEndl()(tmp_str);
                    if (detect_any_char.result)
                    {
                        const auto begin = instrword.parsed_str.data();
                        const auto end   = detect_any_char.rest.data();

                        errors::UnexpectedCharacter error {
                            detect_any_char.parsed_str[0]
                        };

                        return make_parser_error(
                            0,
                            line_count,
                            error,
                            std::string_view(begin,
                                             std::uint32_t(end - begin)));
                    }
                }

                auto skip_to_end = SkipLine()(tmp_str);
                tmp_str          = skip_to_end.rest;
                ++line_count;
            }

            // Sanitize labels
            for (std::int32_t i = 0; i < asl::ssize(instructions); ++i)
            {
                bool error = false;

                for (const auto& op : instructions[i].operands)
                {
                    // clang-format off
                    std::visit(asl::overloaded{
                        [&] (const auto& op) {
                            if constexpr (
                                    details::has_value_var<decltype(op)>::value)
                            {
                                error = !checkLabelsInAST(*op.value, labels);
                            }
                        }
                    }, op);
                    // clang-format on

                    if (error)
                    {
                        errors::NoLabelFound error;
                        return make_parser_error(0,
                                                 instructions[i].sourceLine,
                                                 error,
                                                 instructionStr[i]);
                    }
                }
            }

            return momiji::ParsingInfo { std::move(instructions),
                                         std::move(labels) };
        }

    private:
        void handleBreakpoints()
        {
            // Check if we should insert a breakpoint
            auto found_breakpoint =
                std::find_if(std::begin(settings.breakpoints),
                             std::end(settings.breakpoints),
                             [this](momiji::Breakpoint x) {
                                 return x.source_line == line_count;
                             });

            if (found_breakpoint != std::end(settings.breakpoints))
            {
                momiji::ParsedInstruction breakpoint;
                breakpoint.instructionType = InstructionType::Breakpoint;
                instructions.emplace_back(std::move(breakpoint));
                instructionStr.emplace_back("<BREAKPOINT>");

                program_counter += 4;
            }
        }

        void handlePCIncrement(momiji::ParsedInstruction& instr)
        {
            if (instr.instructionType == InstructionType::Declare)
            {
                switch (instr.dataType)
                {
                case DataType::Byte:
                    program_counter += instr.operands.size();
                    break;

                case DataType::Word:
                    program_counter += instr.operands.size() * 2;
                    break;

                case DataType::Long:
                    program_counter += instr.operands.size() * 4;
                    break;
                }
            }
            else if (isInternal(instr.instructionType))
            {
                program_counter += 4;
            }
            else if (isDirective(instr.instructionType))
            {
                // Intentionally left blank
                // Directives should not increase the program counter
            }
            else if (isBranchInstr(instr.instructionType))
            {
#ifdef LIBMOMIJI_CORRECT_BRA_IMPL
                program_counter += 2;
                if (instr.operands[0].value > 255)
                {
                    program_counter += 2;
                }
#else
                program_counter += 4;
#endif
            }
            else
            {
                program_counter += 2;

                for (const auto& op : instr.operands)
                {
                    program_counter +=
                        requiresImmediateData(op, instr.dataType);
                }
            }
        }

        std::vector<momiji::ParsedInstruction> instructions;
        std::vector<std::string_view> instructionStr;
        LabelInfo labels;

        std::string_view str;

        std::int64_t line_count { 1 };
        std::int64_t program_counter { 0 };

        ParserError last_error;
        ParserSettings settings;
        std::optional<ParserSection> currentSection { std::nullopt };
        bool parsing_error { false };
    };

    momiji::ParsingResult parse(const std::string& str)
    {
        momiji::Parser parser { str };
        return parser.run();
    }

    momiji::ParsingResult parse(const std::string& str, ParserSettings settings)
    {
        momiji::Parser parser { str, settings };
        return parser.run();
    }

    std::int32_t resolveAST(const momiji::objects::MathASTNode& node,
                            const momiji::LabelInfo& labels)
    {
        if (std::holds_alternative<objects::Label>(node.value))
        {
            return visitLabel(std::get<objects::Label>(node.value), labels);
        }

        if (std::holds_alternative<objects::Number>(node.value))
        {
            return visitNum(std::get<objects::Number>(node.value), labels);
        }

        return visitOp(std::get<objects::MathOperator>(node.value), labels);
    }

    momiji::ParserOperand convertOperand(const momiji::Operand& operand)
    {
        namespace ops = momiji::operands;

        momiji::ParserOperand res;

        // clang-format off
        std::visit(asl::overloaded {
            [&](const ops::DataRegister&) {
                res = ParserOperand::DataRegister;
            },

            [&](const ops::AddressRegister&) {
                res = ParserOperand::AddressRegister;
            },

            [&](const ops::Address&) {
                res = ParserOperand::Address;
            },
            
            [&](const ops::AddressPre&) {
                res = ParserOperand::AddressPre;
            },

            [&](const ops::AddressPost&) {
                res = ParserOperand::AddressPost;
            },

            [&](const ops::AddressOffset&) {
                res = ParserOperand::AddressOffset;
            },

            [&](const ops::AddressIndex&) {
                res = ParserOperand::AddressIndex;
            },

            [&](const ops::Immediate&) {
                res = ParserOperand::Immediate;
            },

            [&](const ops::AbsoluteShort&) {
                res = ParserOperand::AbsoluteShort;
            },

            [&](const ops::AbsoluteLong&) {
                res = ParserOperand::AbsoluteLong;
            },

            [&](const ops::ProgramCounterOffset&) {
                res = ParserOperand::ProgramCounterOffset;
            },

            [&](const ops::ProgramCounterIndex&) {
                res = ParserOperand::ProgramCounterIndex;
            },
        }, operand);
        // clang-format on

        return res;
    }

    std::int32_t extractRegister(const Operand& operand)
    {
        momiji::details::RegVisitor visitor;
        std::visit(visitor, operand);

        return visitor.val;
    }

    std::int32_t extractASTValue(const Operand& operand,
                                 const LabelInfo& labels)
    {
        momiji::details::ValueVisitor visitor { labels };
        std::visit(visitor, operand);

        return visitor.val;
    }
} // namespace momiji
