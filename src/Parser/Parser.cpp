#include <Parser.h>

#include <iostream>
#include "Utils.h"
#include <string>
#include <algorithm>
#include "Mappings.h"
#include "Common.h"
#include "Combinators.h"

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

    static bool is_immediate(const Operand& op)
    {
        return op.operandType == OperandType::Immediate &&
               op.specialAddressingMode == SpecialAddressingMode::Immediate;
    }

    struct LabelParser
    {
        LabelParser(std::string_view str)
            : str(str), line_count{0}, label_idx{0}
        { }

        void run()
        {
            auto TryInstr = Sequence(
                    Whitespace(),
                    Map(Word(), [&] (auto&&) { ++label_idx; }));

            std::string_view tmp_str = str;
            while (tmp_str.size() > 0)
            {
                // Ignore all initial spaces
                {
                    auto skip_whitespace = Whitespace()(tmp_str);
                    tmp_str = skip_whitespace.rest;
                }

                auto try_label = ParseLabel()(tmp_str);
                if (try_label.result)
                {
                    // We found a label!
                    auto label_hash = utils::hash(try_label.parsed_str);
                    info.labels.emplace_back(label_hash, label_idx);
                }
                tmp_str = try_label.rest;

                {
                    auto try_instr = TryInstr(tmp_str);
                    tmp_str = try_instr.rest;
                }

                {
                    auto skip_line = SkipLine()(tmp_str);
                    tmp_str = skip_line.rest;
                }

                ++line_count;
            }
        }

        LabelInfo info;
        int line_count;
        int label_idx;
        std::string_view str;
    };

    struct Parser
    {
        Parser(std::string_view str)
            : str(str)
        {
        }

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
                }

                // Maybe we have an instruction?
                auto instrword = Word()(tmp_str);
                tmp_str = instrword.rest;

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

                instr.program_counter = program_counter;
                auto res = found_instr->execfn(tmp_str, instr, labels);

                if (res.result)
                {
                    instructions.push_back(instr);

                    // Every increment of one to program_counter is 2 bytes
                    // Because memory is 16bit-aligned

                    // First 2 bytes for the instruction
                    ++program_counter;

                    if (is_immediate(instr.operands[0]))
                    {
                        switch (instr.dataType)
                        {
                        case DataType::Byte:
                        case DataType::Word:
                            // 2 bytes of immediate data
                            ++program_counter;
                            break;

                        case DataType::Long:
                            // 4 bytes of immediate data
                            program_counter += 2;
                            break;
                        }
                    }
                }

                tmp_str = res.rest;

                // Parsed an instruction!
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
                    op.value = op.value - x.program_counter;
                }
            }

            return instructions;
        }

    private:
        int line_count{0};
        asl::i64 program_counter{0};
        std::string_view str;
        std::vector<momiji::Instruction> instructions;
        LabelInfo labels;
        bool parsing_error{false};
        ParserError last_error;
    };

    momiji::ParsingResult parse(const std::string& str)
    {
        Parser parser{str};

        return parser.run();
    }
}
