#include <momiji/Parser.h>

#include "./Utils.h"
#include "Combinators.h"
#include "Common.h"
#include "Mappings.h"
#include <momiji/Utils.h>

namespace momiji
{
    inline namespace v2
    {
        namespace
        {

            auto make_parser_error(std::int64_t column,
                                   std::int64_t line,
                                   const ParserError::ErrorType& error,
                                   std::string_view sourceCode)
            {
                return nonstd::make_unexpected<ParserError>(
                    { line,
                      column,
                      std::move(error),
                      std::string { sourceCode } });
            }

            bool isInternal(const momiji::v2::ParsedInstruction& instr)
            {
                return instr.instructionType == InstructionType::Breakpoint ||
                       instr.instructionType == InstructionType::HaltCatchFire;
            }

            bool isBranchInstr(const momiji::v2::ParsedInstruction& instr)
            {
                return (instr.instructionType == InstructionType::Branch) ||
                       (instr.instructionType ==
                        InstructionType::BranchCondition) ||
                       (instr.instructionType ==
                        InstructionType::BranchSubroutine);
            }

            int requiresImmediateData(const momiji::v2::Operand& op,
                                      momiji::DataType dataType)
            {
                if (std::holds_alternative<v2::operands::Immediate>(op))
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

                if (std::holds_alternative<v2::operands::AbsoluteShort>(op))
                {
                    return 2;
                }

                if (std::holds_alternative<v2::operands::AbsoluteLong>(op))
                {
                    return 4;
                }

                if (std::holds_alternative<v2::operands::AddressOffset>(op) ||
                    std::holds_alternative<v2::operands::AddressIndex>(op))
                {
                    return 2;
                }

                return 0;
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

            v2::ParsingResult run()
            {
                auto tmp_str = str;
                while (!tmp_str.empty() && !parsing_error)
                {
                    v2::ParsedInstruction instr;

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

                    // Maybe we have an instruction?
                    auto instrword = Word()(tmp_str);
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

                    auto str_hash = momiji::utils::hash(instrword.parsed_str);

                    // Check for a matching instruction
                    auto found_instr =
                        std::find_if(std::begin(momiji::v2::mappings),
                                     std::end(momiji::v2::mappings),
                                     [str_hash](v2::MappingType hash) {
                                         return hash.mapping == str_hash;
                                     });

                    if (found_instr == std::end(momiji::v2::mappings))
                    {
                        auto error        = errors::NoInstructionFound {};
                        error.inputString = instrword.parsed_str;

                        auto begin = instrword.parsed_str.data();
                        auto end = SkipLine()(instrword.rest).parsed_str.data();

                        return make_parser_error(
                            0,
                            line_count,
                            error,
                            std::string_view { begin,
                                               std::uint32_t(end - begin) });
                    }

                    if (!settings.breakpoints.empty())
                    {
                        handleBreakpoints();
                    }

                    auto res = found_instr->execfn(tmp_str, instr);

                    if (res.result)
                    {
                        auto skip_line = SkipLine()(res.rest);

                        const auto begin = instrword.parsed_str.data();
                        const auto end   = skip_line.rest.data();

                        instr.programCounter = std::int32_t(program_counter);

                        instr.sourceLine = std::int32_t(line_count);

                        instructionStr.emplace_back(begin,
                                                    std::size_t(end - begin));

                        instructions.emplace_back(std::move(instr));

                        handlePCIncrement(instr);
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
                            std::string_view(begin,
                                             std::uint32_t(end - begin)));
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

                return v2::ParsingInfo { std::move(instructions),
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
                    momiji::v2::ParsedInstruction breakpoint;
                    breakpoint.instructionType = InstructionType::Breakpoint;
                    instructions.emplace_back(std::move(breakpoint));

                    program_counter += 4;
                }
            }

            void handlePCIncrement(momiji::v2::ParsedInstruction& instr)
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
                else if (isInternal(instr))
                {
                    program_counter += 4;
                }
                else if (isBranchInstr(instr))
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

            std::int64_t line_count { 1 };
            std::int64_t program_counter { 0 };
            std::string_view str;
            std::vector<v2::ParsedInstruction> instructions;
            std::vector<std::string_view> instructionStr;
            LabelInfo labels;
            bool parsing_error { false };
            ParserError last_error;
            ParserSettings settings;
        };

        v2::ParsingResult parse(const std::string& str)
        {
            v2::Parser parser { str };
            return parser.run();
        }

        v2::ParsingResult parse(const std::string& str, ParserSettings settings)
        {
            v2::Parser parser { str, settings };
            return parser.run();
        }

        namespace
        {
            std::int32_t visitNum(const v2::objects::Number& num);
            std::int32_t visitLabel(const v2::objects::Label& label);
            std::int32_t visitOp(const v2::objects::MathOperator& operand);

            std::int32_t visitNum(const v2::objects::Number& num,
                                  const v2::LabelInfo&)
            {
                return num.number;
            }

            std::int32_t visitLabel(const v2::objects::Label& currlabel,
                                    const v2::LabelInfo& labels)
            {
                auto res =
                    std::find_if(std::begin(labels),
                                 std::end(labels),
                                 [&](const v2::Label& label) {
                                     return label.nameHash == currlabel.hash;
                                 });

                if (res == std::end(labels))
                {
                    return 0;
                }

                return std::int32_t(res->idx);
            }

            std::int32_t visitOp(const v2::objects::MathOperator& operand,
                                 const v2::LabelInfo& labels)
            {
                using namespace v2::objects;

                const auto& left  = *operand.left;
                const auto& right = *operand.left;

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
        } // namespace

        std::int32_t resolveAST(const v2::objects::MathASTNode& node,
                                const v2::LabelInfo& labels)
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

        v2::ParserOperand convertOperand(const v2::Operand& operand)
        {
            using namespace momiji::v2;
            namespace ops = momiji::v2::operands;

            momiji::v2::ParserOperand res;

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

        std::int8_t extractRegister(const Operand& operand)
        {
            v2::details::RegVisitor visitor;
            std::visit(visitor, operand);

            return visitor.val;
        }

        std::int32_t extractASTValue(const Operand& operand,
                                     const LabelInfo& labels)
        {
            v2::details::ValueVisitor visitor { labels };
            std::visit(visitor, operand);

            return visitor.val;
        }
    } // namespace v2
} // namespace momiji
