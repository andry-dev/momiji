#include <Parser.h>

#include "Common.h"
#include "Mappings.h"
#include "Utils.h"
#include <algorithm>
#include <string>

#include <Memory.h>

//#include "Instructions.h"
#include <System.h>

#include <asl/types>

#include <momiji/Types.h>

namespace momiji
{
    static auto make_parser_error(std::int64_t column,
                                  std::int64_t line,
                                  const ParserError::ErrorType& error,
                                  std::string_view sourceCode)
    {
        return nonstd::make_unexpected<ParserError>(
            { line, column, std::move(error), std::string { sourceCode } });
    }

    static bool isInternal(const Instruction& instr)
    {
        return instr.instructionType == InstructionType::Breakpoint ||
               instr.instructionType == InstructionType::HaltCatchFire;
    }

    static int requiresImmediateData(const Operand& op,
                                     momiji::DataType dataType)
    {
        if ((op.operandType == OperandType::Immediate) &&
            (op.specialAddressingMode == SpecialAddressingMode::Immediate))
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

        if ((op.operandType == OperandType::Immediate) &&
            (op.specialAddressingMode == SpecialAddressingMode::AbsoluteShort))
        {
            return 2;
        }

        if ((op.operandType == OperandType::Immediate) &&
            (op.specialAddressingMode == SpecialAddressingMode::AbsoluteLong))
        {
            return 4;
        }

        if ((op.operandType == OperandType::AddressOffset) ||
            (op.operandType == OperandType::AddressIndex))
        {
            return 2;
        }

        return 0;
    }

    static bool isBranchInstr(const Instruction& instr)
    {
        return instr.instructionType == InstructionType::Branch ||
               instr.instructionType == InstructionType::BranchCondition ||
               instr.instructionType == InstructionType::BranchSubroutine;
    }

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

        ParsingResult run()
        {
            auto tmp_str = str;
            while (!tmp_str.empty() && !parsing_error)
            {
                momiji::Instruction instr;

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
                    std::find_if(std::begin(momiji::mappings),
                                 std::end(momiji::mappings),
                                 [str_hash](MappingType hash) {
                                     return hash.mapping == str_hash;
                                 });

                if (found_instr == std::end(momiji::mappings))
                {
                    auto error        = errors::NoInstructionFound {};
                    error.inputString = instrword.parsed_str;

                    auto begin = instrword.parsed_str.data();
                    auto end   = SkipLine()(instrword.rest).parsed_str.data();

                    return make_parser_error(
                        0,
                        line_count,
                        error,
                        std::string_view { begin, std::uint32_t(end - begin) });
                }

                if (!settings.breakpoints.empty())
                {
                    handleBreakpoints();
                }

                auto res = found_instr->execfn(tmp_str, instr, labels);

                if (res.result)
                {
                    auto skip_line = SkipLine()(res.rest);

                    const auto begin = instrword.parsed_str.data();
                    const auto end   = skip_line.rest.data();

                    instr.programCounter = program_counter;

                    instr.metadata.sourceLine = line_count;

                    instr.metadata.codeStr = { begin,
                                               std::size_t(end - begin) };

                    instructions.emplace_back(instr);

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

            // All the instructions are parsed now, we should resolve
            // all label resolution issues
            for (auto& x : instructions)
            {
                for (auto& op : x.operands)
                {
                    if (!op.labelResolved)
                    {
                        auto found =
                            alg::find_label(labels, std::uint32_t(op.value));

                        if (found == std::end(labels))
                        {
                            return make_parser_error(0,
                                                     x.metadata.sourceLine,
                                                     errors::NoLabelFound {},
                                                     x.metadata.codeStr);
                        }

                        op.value         = std::int32_t(found->idx);
                        op.labelResolved = true;
                    }
                }
            }

            for (auto& x : instructions)
            {
                // Fix displacements for branch instructions
                if (isBranchInstr(x))
                {
                    auto& op = x.operands[0];
                    op.value = op.value - std::int32_t(x.programCounter);
                }
            }

            return instructions;
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
                momiji::Instruction breakpoint;
                breakpoint.instructionType = InstructionType::Breakpoint;
                instructions.emplace_back(breakpoint);

                program_counter += 4;
            }
        }

        void handlePCIncrement(momiji::Instruction& instr)
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
        std::vector<momiji::Instruction> instructions;
        LabelInfo labels;
        bool parsing_error { false };
        ParserError last_error;
        ParserSettings settings;
    };

    momiji::ParsingResult parse(const std::string& str)
    {
        Parser parser { str };

        return parser.run();
    }

    momiji::ParsingResult parse(const std::string& str, ParserSettings settings)
    {
        Parser parser { str, settings };

        return parser.run();
    }
} // namespace momiji
