#include <Parser.h>

#include "Utils.h"
#include <string>
#include <algorithm>
#include "Mappings.h"
#include "Common.h"
#include "Combinators.h"

#include <Memory.h>

//#include "Instructions.h"
#include <System.h>

#include <asl/types>


namespace momiji
{
    static auto make_parser_error(int column, int line,
            ParserError::ErrorType error)
    {
        return nonstd::make_unexpected<ParserError>({ line, column, error });
    }

    static bool isImmediate(const Operand& op)
    {
        return op.operandType == OperandType::Immediate &&
               (op.specialAddressingMode == SpecialAddressingMode::Immediate ||
                op.specialAddressingMode == SpecialAddressingMode::AbsoluteShort ||
                op.specialAddressingMode == SpecialAddressingMode::AbsoluteLong);
    }

    struct Parser
    {
        Parser(std::string_view str)
            : str(str)
            , settings{ParserSettings{}}
        { }

        Parser(std::string_view str, ParserSettings settings)
            : str(str)
            , settings(settings)
        { }

        ParsingResult run()
        {
            auto tmp_str = str;
            while (tmp_str.size() > 0 && !parsing_error)
            {
                momiji::Instruction instr;

                {
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str = skip_whitespace.rest;
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
                    labels.labels.emplace_back(label_hash, program_counter);
                    tmp_str = try_label.rest;
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str = skip_whitespace.rest;
                }

                // Maybe we have an instruction?
                auto instrword = Word()(tmp_str);
                tmp_str = instrword.rest;

                // If we don't find an instruction, we just skip the line
                if (!instrword.result)
                {
                    auto skip_to_end = SkipLine()(tmp_str);
                    tmp_str = skip_to_end.rest;
                    ++line_count;
                    continue;
                }

                auto str_hash = momiji::utils::hash(instrword.parsed_str);

                // Check for a matching instruction
                auto found_instr = std::find_if(
                    std::begin(momiji::mappings), std::end(momiji::mappings),
                    [str_hash](MappingType hash) {
                        return hash.mapping == str_hash;
                    });

                if (found_instr == std::end(momiji::mappings))
                {
                    return make_parser_error(
                        0, line_count,
                        ParserError::ErrorType::NoInstructionFound);
                }

                if (!settings.breakpoints.empty())
                {
                    handleBreakpoints();
                }

                instr.programCounter = program_counter;
                auto res = found_instr->execfn(tmp_str, instr, labels);

                if (res.result)
                {
                    instructions.emplace_back(instr);

                    handlePCIncrement(instr);
                }
                else
                {
                    return make_parser_error(0, line_count, res.error.errorType);
                }

                tmp_str = res.rest;

                auto skip_to_end = SkipLine()(tmp_str);
                tmp_str = skip_to_end.rest;
                ++line_count;
            }

            // All the instructions are parsed now, we should resolve all
            // label resolution issues
            for (auto& x : instructions)
            {
                for (int i = 0; i < x.operands.size(); ++i)
                {
                    auto& op = x.operands[i];
                    if (!op.labelResolved)
                    {
                        auto found = alg::find_label(labels, x.operands[0].value);

                        if (found == std::end(labels.labels))
                        {
                            return make_parser_error(0, line_count, ParserError::ErrorType::NoLabelFound);
                        }

                        op.value = found->idx;
                        op.labelResolved = true;
                    }
                }
            }

            for (auto& x : instructions)
            {
                // Fix displacements for branch instructions
                if (x.instructionType == InstructionType::BranchCondition ||
                    x.instructionType == InstructionType::Branch)
                {
                    auto& op = x.operands[0];
                    op.value = op.value - x.programCounter;
                }
            }

            return instructions;
        }

    private:

        void handleBreakpoints()
        {
            // Check if we should insert a breakpoint
            auto found_breakpoint = std::find_if(
                std::begin(settings.breakpoints), std::end(settings.breakpoints),
                [this] (momiji::Breakpoint x) {
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
            program_counter += 2;

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
            else
            {
                for (const auto& op : instr.operands)
                {
                    if (isImmediate(op))
                    {
                        switch (instr.dataType)
                        {
                        case DataType::Byte:
                        case DataType::Word:
                            // 2 bytes of immediate data
                            program_counter += 2;
                            break;

                        case DataType::Long:
                            // 4 bytes of immediate data
                            program_counter += 4;
                            break;
                        }
                    }
                }
            }
        }

        std::int64_t line_count{0};
        std::int64_t program_counter{0};
        std::string_view str;
        std::vector<momiji::Instruction> instructions;
        LabelInfo labels;
        bool parsing_error{false};
        ParserError last_error;
        ParserSettings settings;
    };

    momiji::ParsingResult parse(const std::string& str)
    {
        Parser parser{str};

        return parser.run();
    }

    momiji::ParsingResult parse(const std::string& str, ParserSettings settings)
    {
        Parser parser{str, settings};

        return parser.run();
    }
}
