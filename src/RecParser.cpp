#include <Parser.h>

#include <iostream>
#include "Utils.h"
#include <string>

#include "Instructions.h"

namespace momiji
{
    static auto make_parser_error(int column, int line,
            parser_error::error_type error)
    {
        return nonstd::make_unexpected<parser_error>({ line, column, error });
    }

    struct parser_metadata
    {
        bool result;
        std::string_view rest;
        std::string_view parsed_str;
    };

    struct label_info
    {
        std::vector<momiji::label> labels;
    };


    template <typename Parser>
    auto unbox(std::string_view str, Parser&& parser)
    {
        return parser(str);
    }

    template <typename First, typename Second>
    constexpr auto Next(First&& first, Second&& second)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res1 = first(str);
            if (res1.result)
            {
                return second(res1.rest);
            }

            return { false, str, "" };
        };
    }

    template <typename... Ts>
    constexpr auto SeqNext(Ts&&... parsers)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            bool notdone = true;

            parser_metadata res = { false, str, "" };

            ([&] () constexpr {
                if (!notdone)
                {
                    return;
                }

                res = parsers(str);
                notdone = res.result;
                str = res.rest;
             }(), ...);

            return res;
        };
    }

    template <typename... Ts>
    constexpr auto AnyOf(Ts&&... parsers)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            bool done = false;
            parser_metadata res = { false, str, "" };

            ([&] () constexpr -> void {
                if (done)
                {
                    return;
                }

                res = parsers(str);
                done = res.result;
             }(), ...);

            return res;
        };
    }


    template <typename Parser>
    constexpr auto AllOf(Parser&& parser)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            return parser(str);
        };
    }

    template <typename Parser, typename... Ts>
    constexpr auto AllOf(Parser&& parser, Ts&&... rest)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res = parser(str);
            if (res.result)
            {
                return AllOf(rest...)(str);
            }

            return res;
        };
    }

    template <typename... Ts>
    constexpr auto Sequence(Ts&&... parsers)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            parser_metadata res = { false, str, "" };

            ([&] () constexpr {
                res = parsers(str);
                str = res.rest;
             }(), ...);

            return res;
        };
    }

    template <typename Parser>
    constexpr auto While(Parser&& parser)
    {
        return  [=] (std::string_view str) -> parser_metadata
        {
            parser_metadata ret = { false, str, "" };

            auto backup = str;

            int idx = 0;

            while (true)
            {
                auto res = parser(ret.rest);

                if (!res.result)
                {
                    break;
                }

                ++idx;
                ret = res;
            }

            if (idx > 0)
            {
                ret.result = true;
                ret.parsed_str = backup.substr(0, idx);
            }

            return ret;
        };
    }

    // This doesn't actually work.
    /*
    template <typename Parser>
    auto Not(Parser&& parser)
    {
        return [&] (std::string_view str) -> parser_metadata
        {
            auto res = parser(str);

            return { !res.result, str, res.parsed_str };
        };
    }
    */

    template <typename Parser, typename Fun>
    constexpr auto Map(Parser&& parser, Fun&& f)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res = parser(str);

            if (res.result)
            {
                f(res.parsed_str);
            }

            return res;
        };
    }

    constexpr auto Str(std::string_view cmpstr)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            if (str.size() > cmpstr.size())
            {
                auto trim_str = str.substr(0, cmpstr.size());
                if (trim_str == cmpstr)
                {
                    return { true, str.substr(cmpstr.size()), trim_str };
                }
            }

            return { false, str, "" };
        };
    }

    constexpr auto NotStr(std::string_view cmpstr)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            if (str.size() > cmpstr.size())
            {
                auto trim_str = str.substr(0, cmpstr.size());
                if (trim_str != cmpstr)
                {
                    return { true, str.substr(cmpstr.size()), trim_str };
                }
            }

            return { false, str, "" };
        };
    }

    constexpr auto Char(char c)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            if (str.size() > 0 && str[0] == c)
            {
                return { true, str.substr(1), str.substr(0, 1) };
            }

            return { false, str, "" };
        };
    }

    constexpr auto NotChar(char c)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            if (str.size() > 0 && str[0] != c)
            {
                return { true, str.substr(1), str.substr(0, 1) };
            }

            return { false, str, "" };
        };
    }

    template <typename First, typename Second>
    constexpr auto Then(First&& first, Second&& second)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res1 = first(str);

            if (res1.result)
            {
                auto res2 = second(res1.rest);
                if (res2.result)
                {
                    return { true, res2.rest, res1.parsed_str };
                }
            }

            return { false, str,  "" };
        };
    }

    template <typename First, typename Second>
    constexpr auto Optional(First&& first, Second&& second)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res1 = first(str);

            if (res1.result)
            {
                auto res2 = second(res1.rest);
                if (res2.result)
                {
                    return res2;
                }

                return res1;
            }

            return { false, str, "" };
        };
    }

    template <typename Delim1, typename Delim2, typename Body>
    constexpr auto Between(Delim1&& head, Body&& body, Delim2&& tail)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto head_res = head(str);
            if (head_res.result)
            {
                return Then(body, tail)(head_res.rest);
            }

            return { false, str, "" };
        };
    }

    template <typename Parser>
    constexpr auto AlwaysTrue(Parser&& parser)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            auto res = parser(str);
            return { true, res.rest, res.parsed_str };
        };
    }

    constexpr auto Endl()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            const bool check_size = str.size() == 0;

            if (check_size)
            {
                return { true, str, "" };
            }

            return AnyOf(Char('\n'), Char('\0'), Char('\r'))(str);
        };
    }

    constexpr auto NotEndl()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            const bool check_size = str.size() == 0;

            if (check_size)
            {
                return { false, str, "" };
            }


            return AllOf(NotChar('\n'), NotChar('\0'), NotChar('\r'))(str);
        };
    }

    constexpr auto Whitespace()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            return While(AnyOf(Char(' '), Char('\t')))(str);
        };
    }

    constexpr auto Number()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            switch (str[0])
            {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                return { true, str.substr(1), str.substr(0, 1) };

            default:
                return { false, str, "" };
            };
        };
    }

    constexpr auto AsciiAlphabet()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            if (str.size() > 0)
            {
                if ((str[0] >= 'a' && str[0] <= 'z') ||
                    (str[0] >= 'A' && str[0] <= 'Z') ||
                    (str[0] >= '0' && str[0] <= '9'))
                {
                    return { true, str.substr(1), str.substr(0, 1) };
                }
            }

            return { false, str, "" };
        };
    }

    constexpr auto Word()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            return While(AsciiAlphabet())(str);
        };
    }

    constexpr auto SkipLine()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            return Sequence(While(NotEndl()), Endl())(str);
        };
    }

    constexpr auto Comment()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            return Next(Char(';'), SkipLine())(str);
        };
    }

    constexpr auto SkipSeparator(char c)
    {
        return [=] (std::string_view str) -> parser_metadata
        {
            return Sequence(Whitespace(), Char(c), Whitespace())(str);
        };
    }

    constexpr auto Label()
    {
        return [] (std::string_view str) -> parser_metadata
        {
            return Then(Word(), Char(':'))(str);
        };
    }

    constexpr auto OperandImmediate(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto number_parser = While(Number());
            constexpr auto inter_dec_parser = SeqNext(Char('#'), number_parser);

            auto decimal_num =
                Map(inter_dec_parser,
                    [&instr, opNum] (auto parsed_str) {
                        const std::int32_t val = std::stoi(std::string{parsed_str});

                        instr.operands[opNum].operandType = operand_type::Immediate;
                        instr.operands[opNum].value = val;
                    });

            constexpr auto inter_hex_parser = SeqNext(Char('#'), Char('$'), number_parser);
            auto hex_num =
                Map(inter_hex_parser,
                    [&instr, opNum] (auto parsed_str) {
                        const std::int32_t val = std::stoi(std::string{parsed_str}, 0, 16);

                        instr.operands[opNum].operandType = operand_type::Immediate;
                        instr.operands[opNum].value = val;
                    });

            return AnyOf(decimal_num, hex_num)(str);
        };
    }

    constexpr auto AddressRegister(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser = SeqNext(Char('a'), Number());
            auto register_parser =
                Map(inter_parser, [&] (auto parsed_str) {
                    const int reg_num = std::stoi(std::string{parsed_str});

                    instr.operands[opNum].operandType = operand_type::Register;
                    instr.operands[opNum].value = reg_num;
                    instr.operands[opNum].registerType = register_type::Address;
                });

            return register_parser(str);
        };
    }

    constexpr auto DataRegister(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser = SeqNext(Char('d'), Number());
            auto register_parser =
                Map(inter_parser,
                    [&] (std::string_view parsed_str) {
                        const int reg_num = std::stoi(std::string{parsed_str});

                        instr.operands[opNum].operandType = operand_type::Register;
                        instr.operands[opNum].value = reg_num;
                        instr.operands[opNum].registerType = register_type::Data;
                    });

            return register_parser(str);
        };
    }

    constexpr auto AnyRegister(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser = AnyOf(DataRegister(instr, opNum),
                                         AddressRegister(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AnyOperand(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser = AnyOf(OperandImmediate(instr, opNum),
                                         AnyRegister(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AsAddress(momiji::instruction& instr, int opNum)
    {
        return [&instr, opNum] (std::string_view str) -> parser_metadata
        {
            auto register_parser =
                Map(Between(Char('('), AnyRegister(instr, opNum), Char(')')),
                    [&] (std::string_view parsed_str) {
                        instr.operands[opNum].operandType = operand_type::Address;
                    });

            return register_parser(str);
        };
    }

    constexpr auto DataType(momiji::instruction& instr)
    {
        return [&instr] (std::string_view str) -> parser_metadata
        {
            constexpr auto inter_parser =
                Next(Char('.'),
                     AnyOf(Char('b'), Char('w'), Char('l')));

            auto data_type_parser =
                Map(inter_parser,
                    [&] (std::string_view parsed_str) {
                        switch (parsed_str[0])
                        {
                        case 'b':
                            instr.dataType = data_type::Byte;
                            break;
                        case 'w':
                            instr.dataType = data_type::Word;
                            break;
                        case 'l':
                            instr.dataType = data_type::Long;
                            break;
                        }
                    });

            return data_type_parser(str);
        };
    }

    constexpr auto CommonInstructionParser(momiji::instruction& instr)
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

    constexpr auto ImmediateInstructionParser(momiji::instruction& instr)
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

    struct LabelParser
    {
        LabelParser(std::string_view str)
            : str(str), line_count{0}
        { }

        void run()
        {
            std::string_view tmp_str = str;
            while (tmp_str.size() > 0)
            {
                // Ignore all initial spaces
                auto skip_whitespace = Whitespace()(tmp_str);
                tmp_str = skip_whitespace.rest;

                auto try_label = Label()(tmp_str);
                if (try_label.result)
                {
                    // We found a label!
                    auto label_hash = momiji::utils::hash(try_label.parsed_str);
                    info.labels.emplace_back(label_hash, line_count);
                    tmp_str = try_label.rest;
                }

                {
                    auto skip_line = SkipLine()(tmp_str);
                    tmp_str = skip_line.rest;
                }
                ++line_count;

                //std::cout << "\n--- LOOP END ---\n" << tmp_str << '\n';
            }
        }

        label_info info;
        int line_count;
        std::string_view str;
    };

    struct Parser
    {
        Parser(std::string_view str, const label_info& lbl)
            : str(str)
            , labels(lbl)
        {
        }

        parsing_result run()
        {
            auto tmp_str = str;
            while (tmp_str.size() > 0 && !parsing_error)
            {
                momiji::instruction instr;

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
                    auto skip_labels = Sequence(Label(), Whitespace())(tmp_str);
                    tmp_str = skip_labels.rest;
                }

                // Maybe we have an instruction?
                auto instrword = Word()(tmp_str);
                tmp_str = instrword.rest;
                std::cout << instrword.parsed_str << '\n';
                auto str_hash = momiji::utils::hash(instrword.parsed_str);

                parser_metadata res = { };
                switch (str_hash)
                {
                case utils::hash("move"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::Move;
                    instr.executefn = op_impl::move[utils::to_val(instr.dataType)];
                    break;

                case utils::hash("moveq"):
                    res = ImmediateInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::MoveImmediate;
                    instr.executefn = op_impl::moveq;
                    break;

                case utils::hash("add"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::Add;
                    instr.executefn = op_impl::add[utils::to_val(instr.dataType)];
                    break;

                case utils::hash("sub"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::Sub;
                    instr.executefn = op_impl::sub[utils::to_val(instr.dataType)];
                    break;

                case utils::hash("muls"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::SignedMul;
                    instr.executefn = op_impl::muls;
                    break;

                case utils::hash("mulu"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::UnsignedMul;
                    instr.executefn = op_impl::muls;
                    break;

                case utils::hash("divs"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::SignedDiv;
                    instr.executefn = op_impl::divs;
                    break;

                case utils::hash("divu"):
                    res = CommonInstructionParser(instr)(tmp_str);
                    instr.numOperands = 2;
                    instr.instructionType = instruction_type::UnsignedDiv;
                    instr.executefn = op_impl::divu;
                    break;



                default:
                    return make_parser_error(0,
                                             line_count,
                                             parser_error::error_type::NoInstructionFound);

                    break;
                }

                if (!res.result)
                {
                    // Ok, maybe there's a label without an instruction
                    parsing_error = true;
                    return make_parser_error(0,
                                             line_count,
                                             parser_error::error_type::UnexpectedCharacter);
                }

                tmp_str = res.rest;
                instructions.push_back(instr);

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
        std::vector<momiji::instruction> instructions;
        const label_info& labels;
        bool parsing_error{false};
        parser_error last_error;
    };

    momiji::parsing_result parse(const std::string& str)
    {
        LabelParser label{str};

        label.run();

        Parser parser{str, label.info};

        auto whatever = parser.run();

        if (whatever)
        {
            for (const auto& inst : (*whatever))
            {
                std::cout << "Found instruction!\n";
                std::cout << "Val " << std::to_string(inst.operands[0].value) << '\n';
            }
        }

        return whatever;
    }
}
