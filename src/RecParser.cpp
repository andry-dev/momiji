#include <Parser.h>

#include <iostream>
#include "Utils.h"
#include <string>
#include <algorithm>

//#include "Instructions.h"
#include <System.h>

#include "ParserCombinators.h"

namespace momiji
{
    static auto make_parser_error(int column, int line,
            ParserError::ErrorType error)
    {
        return nonstd::make_unexpected<ParserError>({ line, column, error });
    }


    struct LabelInfo
    {
        std::vector<momiji::Label> labels;
    };



    constexpr auto OperandImmediate(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_dec_parser = SeqNext(Char('#'), GenericDecimal());

            auto decimal_num =
                Map(inter_dec_parser,
                    [&instr, opNum] (auto parsed_str) {
                        const std::int32_t val = std::stoi(std::string{parsed_str});

                        instr.operands[opNum].operandType = OperandType::Immediate;
                        instr.operands[opNum].specialAddressingMode = SpecialAddressingMode::Immediate;
                        instr.operands[opNum].value = val;
                    });

            constexpr auto inter_hex_parser = SeqNext(Char('#'), GenericHex());
            auto hex_num =
                Map(inter_hex_parser,
                    [&instr, opNum] (auto parsed_str) {
                        const std::int32_t val = std::stoi(std::string{parsed_str}, 0, 16);

                        instr.operands[opNum].operandType = OperandType::Immediate;
                        instr.operands[opNum].specialAddressingMode = SpecialAddressingMode::Immediate;
                        instr.operands[opNum].value = val;
                    });

            return AnyOf(decimal_num, hex_num)(str);
        };
    }

    constexpr auto AddressRegisterParser(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser = SeqNext(Char('a'), Number());
            auto register_parser =
                Map(inter_parser, [&] (auto parsed_str) {
                    const int reg_num = std::stoi(std::string{parsed_str});

                    instr.operands[opNum].operandType = OperandType::AddressRegister;
                    instr.operands[opNum].value = reg_num;
                });

            return register_parser(str);
        };
    }

    constexpr auto DataRegisterParser(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser = SeqNext(Char('d'), Number());
            auto register_parser =
                Map(inter_parser,
                    [&] (std::string_view parsed_str) {
                        const int reg_num = std::stoi(std::string{parsed_str});

                        instr.operands[opNum].operandType = OperandType::DataRegister;
                        instr.operands[opNum].value = reg_num;
                    });

            return register_parser(str);
        };
    }

    constexpr auto AnyRegister(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser = AnyOf(DataRegisterParser(instr, opNum),
                                         AddressRegisterParser(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AnyOperand(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser = AnyOf(OperandImmediate(instr, opNum),
                                         AnyRegister(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AsAddress(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser =
                Map(Between(Char('('), AnyRegister(instr, opNum), Char(')')),
                    [&] (std::string_view parsed_str) {
                        instr.operands[opNum].operandType = OperandType::Address;
                    });

            return register_parser(str);
        };
    }

    constexpr auto DataType(momiji::Instruction& instr)
    {
        return [&instr] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser =
                Next(Char('.'),
                     AnyOf(Char('b'), Char('w'), Char('l')));

            auto DataType_parser =
                Map(inter_parser,
                    [&] (std::string_view parsed_str) {
                        switch (parsed_str[0])
                        {
                        case 'b':
                            instr.dataType = DataType::Byte;
                            break;
                        case 'w':
                            instr.dataType = DataType::Word;
                            break;
                        case 'l':
                            instr.dataType = DataType::Long;
                            break;
                        }
                    });

            return DataType_parser(str);
        };
    }

    constexpr auto ResolveLabel(momiji::Instruction& instr, const LabelInfo& labels)
    {
        return [&instr, &labels] (std::string_view str) -> parser_metadata
        {
            auto res = Word()(str);
            if (res.result)
            {
                const auto str_hash = utils::hash(res.parsed_str);

                auto begin = std::begin(labels.labels);
                auto end = std::end(labels.labels);

                auto it = std::find_if(
                        begin, end, [&] (const momiji::Label& x) {
                    return x.name_hash == str_hash; 
                });

                if (it != std::end(labels.labels))
                {
                    instr.operands[0].value = it->idx;
                    instr.operands[0].operandType = OperandType::AbsoluteLong;
                }
                else
                {
                    return { false, str, "" };
                }
            }

            return res;
        };
    }

    constexpr auto CommonInstructionParser(momiji::Instruction& instr)
    {
        return [&instr] (std::string_view str) -> parser_metadata
        {
            auto parser = SeqNext(
                    AlwaysTrue(DataType(instr)), // data type is optional
                    Whitespace(),
                    AnyOperand(instr, 0),
                    AlwaysTrue(Whitespace()),
                    Char(','),
                    AlwaysTrue(Whitespace()),
                    AnyRegister(instr, 1));

            return parser(str);
        };
    }

    constexpr auto ImmediateInstructionParser(momiji::Instruction& instr)
    {
        return [&instr] (std::string_view str) -> parser_metadata
        {
            auto parser = SeqNext(
                    AlwaysTrue(DataType(instr)),
                    Whitespace(),
                    OperandImmediate(instr, 0),
                    AlwaysTrue(Whitespace()),
                    Char(','),
                    AlwaysTrue(Whitespace()),
                    AnyRegister(instr, 1));

            return parser(str);
        };
    }

    constexpr auto BranchInstructionParser(momiji::Instruction& instr,
                                           const momiji::LabelInfo& labels)
    {
        return [&instr, &labels] (std::string_view str) -> parser_metadata
        {
            auto parser = SeqNext(
                    Whitespace(),
                    ResolveLabel(instr, labels));

            return parser(str);
        };
    }

    constexpr auto OneRegisterInstructionParser(momiji::Instruction& instr)
    {
        return [&instr] (std::string_view str) -> parser_metadata
        {
            auto parser = SeqNext(Whitespace(), AnyRegister(instr, 0));

            return parser(str);
        };
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
                case utils::hash("move"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::AddressRegister:
                        instr.instructionType = InstructionType::MoveAddress;
                        break;

                    default:
                        instr.instructionType = InstructionType::Move;
                        break;
                    }
                    break;

                case utils::hash("moveq"):
                    res = ImmediateInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::MoveQuick;
                    break;

                case utils::hash("add"):
                case utils::hash("adda"):
                case utils::hash("addi"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    switch (instr.operands[1].operandType)
                    {
                    case OperandType::AddressRegister:
                        instr.instructionType = InstructionType::AddA;
                        break;

                    default:
                        instr.instructionType = InstructionType::Add;
                        break;
                    }

                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::Immediate:
                        switch (instr.operands[0].specialAddressingMode)
                        {
                        case SpecialAddressingMode::Immediate:
                            instr.instructionType = InstructionType::AddI;
                            break;
                        }
                        break;
                    }

                    break;

                case utils::hash("sub"):
                case utils::hash("suba"):
                case utils::hash("subi"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::Sub;
                    switch (instr.operands[1].operandType)
                    {
                    case OperandType::AddressRegister:
                        instr.instructionType = InstructionType::SubA;
                        break;

                    default:
                        instr.instructionType = InstructionType::Sub;
                        break;
                    }

                    switch (instr.operands[0].operandType)
                    {
                    case OperandType::Immediate:
                        switch (instr.operands[0].specialAddressingMode)
                        {
                        case SpecialAddressingMode::Immediate:
                            instr.instructionType = InstructionType::SubI;
                            break;
                        }
                        break;
                    }
                    break;

                case utils::hash("muls"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::SignedMul;
                    break;

                case utils::hash("mulu"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.instructionType = InstructionType::UnsignedMul;
                    break;

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

                case utils::hash("bra"):
                    res = BranchInstructionParser(instr, labels)(tmp_str);
                    if (!res.result)
                    {
                        return make_parser_error(0, line_count,
                                                 ParserError::ErrorType::NoLabelFound);
                    }

                    instr.instructionType = InstructionType::Jmp;
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
