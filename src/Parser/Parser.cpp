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


namespace momiji
{
    static auto make_parser_error(int column, int line,
            ParserError::ErrorType error)
    {
        return nonstd::make_unexpected<ParserError>({ line, column, error });
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
        Parser(std::string_view str, const LabelInfo& lbl)
            : str(str)
            , labels(lbl)
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

                {
                    auto skip_labels = Sequence(ParseLabel(), Whitespace())(tmp_str);
                    tmp_str = skip_labels.rest;
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

                parser_metadata res = { };
                switch (str_hash)
                {
                case utils::hash("divs"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::SignedDiv;
                    break;

                case utils::hash("divu"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::UnsignedDiv;
                    break;

                case utils::hash("swap"):
                    res = OneRegisterInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::Swap;
                    break;

                case utils::hash("exg"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::Exchange;
                    break;

                case utils::hash("or"):
                case utils::hash("ori"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::Immediate:
                        switch (instr.operands[0].specialAddressingMode)
                        {
                        case SpecialAddressingMode::Immediate:
                            instr.instructionType = InstructionType::OrI;
                            break;
                        }
                        break;

                    default:
                        instr.instructionType = InstructionType::Or;
                    }
                    break;

                case utils::hash("and"):
                case utils::hash("andi"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::Immediate:
                        switch (instr.operands[0].specialAddressingMode)
                        {
                        case SpecialAddressingMode::Immediate:
                            instr.instructionType = InstructionType::AndI;
                            break;
                        }
                        break;

                    default:
                        instr.instructionType = InstructionType::And;
                    }
                    break;

                case utils::hash("cmp"):
                case utils::hash("cmpi"):
                case utils::hash("cmpa"):
                    res = CommonInstructionParser(instr)(tmp_str);

                    switch (instr.operands[1].operandType)
                    {
                    case OperandType::AddressRegister:
                        instr.instructionType = InstructionType::CompareA;
                        break;

                    default:
                        instr.instructionType = InstructionType::Compare;
                    }

                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::Immediate:
                        switch (instr.operands[0].specialAddressingMode)
                        {
                        case SpecialAddressingMode::Immediate:
                            instr.instructionType = InstructionType::CompareI;
                            break;
                        }
                        break;
                    }

                    break;

                case utils::hash("jmp"):
                    res = BranchInstructionParser(instr, labels)(tmp_str);
                    if (!res.result)
                    {
                        return make_parser_error(0, line_count,
                                                 ParserError::ErrorType::NoLabelFound);
                    }

                    instr.instructionType = InstructionType::Jmp;
                    break;

                case utils::hash("bra"):
                    res = BranchInstructionParser(instr, labels)(tmp_str);
                    if (!res.result)
                    {
                        return make_parser_error(0, line_count,
                                                 ParserError::ErrorType::NoLabelFound);
                    }

                    instr.instructionType = InstructionType::Branch;
                    break;

                case utils::hash("ble"):
                case utils::hash("blt"):
                case utils::hash("bge"):
                case utils::hash("bgt"):
                case utils::hash("beq"):
                case utils::hash("bne"):
                    res = BranchInstructionParser(instr, labels)(tmp_str);
                    if (!res.result)
                    {
                        return make_parser_error(0, line_count,
                                                 ParserError::ErrorType::NoLabelFound);
                    }

                    // Mama, Papa, I feel so sorry for this.
                    switch (str_hash)
                    {
                    case utils::hash("ble"):
                        instr.branchCondition = BranchConditions::LessEq;
                        break;

                    case utils::hash("blt"):
                        instr.branchCondition = BranchConditions::LessThan;
                        break;

                    case utils::hash("bge"):
                        instr.branchCondition = BranchConditions::GreaterEq;
                        break;

                    case utils::hash("bgt"):
                        instr.branchCondition = BranchConditions::GreaterThan;
                        break;

                    case utils::hash("beq"):
                        instr.branchCondition = BranchConditions::Equal;
                        break;

                    case utils::hash("bne"):
                        instr.branchCondition = BranchConditions::NotEqual;
                        break;
                    }

                    instr.instructionType = InstructionType::BranchCondition;
                    break;

                default:
                    return make_parser_error(0, line_count,
                                             ParserError::ErrorType::NoInstructionFound);

                    break;
                }

                if (res.result)
                {
                    instructions.push_back(instr);
                }

                tmp_str = res.rest;

                // Parsed an instruction!
                auto skip_to_end = SkipLine()(tmp_str);
                tmp_str = skip_to_end.rest;
                ++line_count;
            }

            return instructions;
        }

    private:
        int line_count{0};
        std::string_view str;
        std::vector<momiji::Instruction> instructions;
        const LabelInfo& labels;
        bool parsing_error{false};
        ParserError last_error;
    };

    momiji::ParsingResult parse(const std::string& str)
    {
        LabelParser label{str};

        label.run();

        Parser parser{str, label.info};

        return parser.run();
    }
}
