#pragma once

#include <Parser.h>

#include <System.h>
#include <algorithm>
#include <string>
#include <string_view>

#include "Common.h"

namespace momiji
{
    constexpr auto AsAddress(momiji::Instruction& instr, int opNum);

    namespace alg
    {
        inline auto find_label(const LabelInfo& labels, std::uint32_t hash)
        {
            auto begin = std::begin(labels.labels);
            auto end   = std::end(labels.labels);

            auto it = std::find_if(begin, end, [&](const momiji::Label& x) {
                return x.name_hash == hash;
            });

            return it;
        }
    } // namespace alg

    template <typename Parser>
    auto unbox(std::string_view str, Parser&& parser)
    {
        return parser(str);
    }

    template <typename First, typename Second>
    constexpr auto Next(First&& first, Second&& second)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res1 = first(str);
            if (res1.result)
            {
                return second(res1.rest);
            }

            return { false, str, "", {} };
        };
    }

    template <typename... Ts>
    constexpr auto SeqNext(Ts&&... parsers)
    {
        return [=](std::string_view str) -> parser_metadata {
            bool notdone = true;

            parser_metadata res = { false, str, "", {} };

            (
                [&]() constexpr {
                    if (!notdone)
                    {
                        return;
                    }

                    res     = parsers(str);
                    notdone = res.result;
                    str     = res.rest;
                }(),
                ...);

            return res;
        };
    }

    template <typename... Ts>
    constexpr auto AnyOf(Ts&&... parsers)
    {
        return [=](std::string_view str) -> parser_metadata {
            bool done           = false;
            parser_metadata res = { false, str, "", {} };

            (
                [&]() constexpr->void {
                    if (done)
                    {
                        return;
                    }

                    res  = parsers(str);
                    done = res.result;
                }(),
                ...);

            return res;
        };
    }

    template <typename Parser>
    constexpr auto AllOf(Parser&& parser)
    {
        return [=](std::string_view str) -> parser_metadata {
            return parser(str);
        };
    }

    template <typename Parser, typename... Ts>
    constexpr auto AllOf(Parser&& parser, Ts&&... rest)
    {
        return [=](std::string_view str) -> parser_metadata {
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
        return [=](std::string_view str) -> parser_metadata {
            parser_metadata res = { false, str, "", {} };

            (
                [&]() constexpr {
                    res = parsers(str);
                    str = res.rest;
                }(),
                ...);

            return res;
        };
    }

    template <typename Parser>
    constexpr auto While(Parser&& parser)
    {
        return [=](std::string_view str) -> parser_metadata {
            parser_metadata ret = { false, str, "", {} };

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
                ret.result     = true;
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
        return [=](std::string_view str) -> parser_metadata {
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
        return [=](std::string_view str) -> parser_metadata {
            if (str.size() > cmpstr.size())
            {
                auto trim_str = str.substr(0, cmpstr.size());
                if (trim_str == cmpstr)
                {
                    return { true, str.substr(cmpstr.size()), trim_str, {} };
                }
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto NotStr(std::string_view cmpstr)
    {
        return [=](std::string_view str) -> parser_metadata {
            if (str.size() > cmpstr.size())
            {
                auto trim_str = str.substr(0, cmpstr.size());
                if (trim_str != cmpstr)
                {
                    return { true, str.substr(cmpstr.size()), trim_str };
                }
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto Char(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            if (str.size() > 0 && str[0] == c)
            {
                return { true, str.substr(1), str.substr(0, 1), {} };
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto NotChar(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            if (str.size() > 0 && str[0] != c)
            {
                return { true, str.substr(1), str.substr(0, 1) };
            }

            return { false, str, "", {} };
        };
    }

    template <typename First, typename Second>
    constexpr auto Then(First&& first, Second&& second)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res1 = first(str);

            if (res1.result)
            {
                auto res2 = second(res1.rest);
                if (res2.result)
                {
                    return { true, res2.rest, res1.parsed_str };
                }
            }

            return { false, str, "", {} };
        };
    }

    template <typename First, typename Second>
    constexpr auto Optional(First&& first, Second&& second)
    {
        return [=](std::string_view str) -> parser_metadata {
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

            return { false, str, "", {} };
        };
    }

    template <typename Delim1, typename Delim2, typename Body>
    constexpr auto Between(Delim1&& head, Body&& body, Delim2&& tail)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto head_res = head(str);
            if (head_res.result)
            {
                return Then(body, tail)(head_res.rest);
            }

            return { false, str, "", {} };
        };
    }

    template <typename Parser>
    constexpr auto AlwaysTrue(Parser&& parser)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res = parser(str);
            return { true, res.rest, res.parsed_str };
        };
    }

    constexpr auto Endl()
    {
        return [](std::string_view str) -> parser_metadata {
            const bool check_size = str.size() == 0;

            if (check_size)
            {
                return { true, str, "", {} };
            }

            return AnyOf(Char('\n'), Char('\0'), Char('\r'))(str);
        };
    }

    constexpr auto NotEndl()
    {
        return [](std::string_view str) -> parser_metadata {
            const bool check_size = str.size() == 0;

            if (check_size)
            {
                return { false, str, "", {} };
            }

            return AllOf(NotChar('\n'), NotChar('\0'), NotChar('\r'))(str);
        };
    }

    constexpr auto Whitespace()
    {
        return [](std::string_view str) -> parser_metadata {
            return While(AnyOf(Char(' '), Char('\t')))(str);
        };
    }

    constexpr auto Number()
    {
        return [](std::string_view str) -> parser_metadata {
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
                return { true, str.substr(1), str.substr(0, 1), {} };

            default:
                return { false, str, "", {} };
            };
        };
    }

    constexpr auto DecNumber()
    {
        return [](std::string_view str) -> parser_metadata {
            int idx        = 0;
            bool found_num = false;

            auto check_sign =
                Map(AnyOf(Char('+'), Char('-')), [&idx](auto) { ++idx; })(str);

            auto check_size   = [&]() { return str.size() > idx; };
            auto check_number = [&]() {
                return (str[idx] >= '0' && str[idx] <= '9');
            };

            while (check_size() && check_number())
            {
                found_num = true;
                ++idx;
            }

            if (found_num)
            {
                return { true, str.substr(idx), str.substr(0, idx), {} };
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto HexNumber()
    {
        return [](std::string_view str) -> parser_metadata {
            int idx = 0;

            auto check_sign =
                Map(AnyOf(Char('+'), Char('-')), [&idx](auto) { ++idx; })(str);

            auto check_size   = [&]() { return idx < str.size(); };
            auto check_number = [&]() {
                return (str[idx] >= '0' && str[idx] <= '9') ||
                       (str[idx] >= 'a' && str[idx] <= 'f') ||
                       (str[idx] >= 'A' && str[idx] <= 'F');
            };

            bool found_num = false;

            while (check_size() && check_number())
            {
                found_num = true;
                ++idx;
            }

            if (found_num)
            {
                return { true, str.substr(idx), str.substr(0, idx), {} };
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto AsciiAlphabet()
    {
        return [](std::string_view str) -> parser_metadata {
            if (str.size() > 0)
            {
                if ((str[0] >= 'a' && str[0] <= 'z') ||
                    (str[0] >= 'A' && str[0] <= 'Z') ||
                    (str[0] >= '0' && str[0] <= '9') || (str[0] == '_'))
                {
                    return { true, str.substr(1), str.substr(0, 1), {} };
                }
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto GenericDecimal()
    {
        return [](std::string_view str) -> parser_metadata {
            return DecNumber()(str);
        };
    }

    constexpr auto GenericHex()
    {
        return [](std::string_view str) -> parser_metadata {
            return SeqNext(Char('$'), HexNumber())(str);
        };
    }

    constexpr auto Word()
    {
        return [](std::string_view str) -> parser_metadata {
            return While(AsciiAlphabet())(str);
        };
    }

    constexpr auto SkipLine()
    {
        return [](std::string_view str) -> parser_metadata {
            return Sequence(While(NotEndl()), Endl())(str);
        };
    }

    constexpr auto Comment()
    {
        return [](std::string_view str) -> parser_metadata {
            return Next(Char(';'), SkipLine())(str);
        };
    }

    constexpr auto SkipSeparator(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            return Sequence(Whitespace(), Char(c), Whitespace())(str);
        };
    }

    constexpr auto ParseLabel()
    {
        return [](std::string_view str) -> parser_metadata {
            return Then(Word(), Char(':'))(str);
        };
    }

    constexpr auto OperandImmediate(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            constexpr auto inter_dec_parser =
                SeqNext(Char('#'), GenericDecimal());

            auto decimal_num =
                Map(inter_dec_parser, [&instr, opNum](auto parsed_str) {
                    const std::int32_t val =
                        std::stoll(std::string { parsed_str });

                    instr.operands[opNum].operandType = OperandType::Immediate;
                    instr.operands[opNum].specialAddressingMode =
                        SpecialAddressingMode::Immediate;
                    instr.operands[opNum].value = val;
                });

            constexpr auto inter_hex_parser = SeqNext(Char('#'), GenericHex());
            auto hex_num                    = Map(
                inter_hex_parser, [&instr, opNum](std::string_view parsed_str) {
                    const std::int32_t val =
                        std::stoll(std::string { parsed_str }, 0, 16);

                    instr.operands[opNum].operandType = OperandType::Immediate;
                    instr.operands[opNum].specialAddressingMode =
                        SpecialAddressingMode::Immediate;
                    instr.operands[opNum].value = val;
                });

            // Example: move.w #arr, a0
            //          ^ moves the "address" of "arr" in a0
            constexpr auto inter_imm_label_parser = SeqNext(Char('#'), Word());
            auto imm_label =
                Map(inter_imm_label_parser, [&instr, opNum](auto parsed_str) {
                    instr.operands[opNum].operandType = OperandType::Immediate;
                    instr.operands[opNum].specialAddressingMode =
                        SpecialAddressingMode::Immediate;
                    instr.operands[opNum].value = utils::hash(parsed_str);
                    instr.operands[opNum].labelResolved = false;
                });

            return AnyOf(decimal_num, hex_num, imm_label)(str);
        };
    }

    constexpr auto AddressRegisterParser(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            constexpr auto inter_parser = SeqNext(Char('a'), DecNumber());
            auto register_parser = Map(inter_parser, [&](auto parsed_str) {
                const int reg_num = std::stoi(std::string { parsed_str });

                instr.operands[opNum].operandType =
                    OperandType::AddressRegister;
                instr.operands[opNum].value = reg_num;
            });

            return register_parser(str);
        };
    }

    constexpr auto DataRegisterParser(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            constexpr auto inter_parser = SeqNext(Char('d'), DecNumber());
            auto register_parser =
                Map(inter_parser, [&](std::string_view parsed_str) {
                    const int reg_num = std::stoi(std::string { parsed_str });

                    instr.operands[opNum].operandType =
                        OperandType::DataRegister;
                    instr.operands[opNum].value = reg_num;
                });

            return register_parser(str);
        };
    }

    constexpr auto MemoryAddress(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            constexpr auto inter_dec_parser = GenericDecimal();

            auto dec_mem =
                Map(inter_dec_parser, [&instr, opNum](auto parsed_str) {
                    const std::int32_t val =
                        std::stoll(std::string { parsed_str });

                    SpecialAddressingMode res_add_mode {};

                    switch (instr.dataType)
                    {
                    case DataType::Byte:
                    case DataType::Word:
                        res_add_mode = SpecialAddressingMode::AbsoluteShort;
                        break;

                    case DataType::Long:
                        res_add_mode = SpecialAddressingMode::AbsoluteLong;
                        break;
                    }

                    instr.operands[opNum].operandType =
                        OperandType::AbsoluteLong;
                    instr.operands[opNum].specialAddressingMode = res_add_mode;
                    instr.operands[opNum].value                 = val;
                });

            constexpr auto inter_hex_parser = GenericHex();
            auto hex_mem =
                Map(inter_hex_parser, [&instr, opNum](auto parsed_str) {
                    const std::int32_t val =
                        std::stoll(std::string { parsed_str }, 0, 16);

                    SpecialAddressingMode res_add_mode {};

                    switch (instr.dataType)
                    {
                    case DataType::Byte:
                    case DataType::Word:
                        res_add_mode = SpecialAddressingMode::AbsoluteShort;
                        break;
                    case DataType::Long:
                        res_add_mode = SpecialAddressingMode::AbsoluteLong;
                        break;
                    }

                    instr.operands[opNum].operandType =
                        OperandType::AbsoluteLong;
                    instr.operands[opNum].specialAddressingMode = res_add_mode;
                    instr.operands[opNum].value                 = val;
                });

            // Example: move.w arr, a0
            //          ^ moves the value at address "arr" in a0
            constexpr auto inter_mem_label_parser = Word();
            auto label_mem =
                Map(inter_mem_label_parser, [&instr, opNum](auto parsed_str) {
                    SpecialAddressingMode res_add_mode {};

                    switch (instr.dataType)
                    {
                    case DataType::Byte:
                    case DataType::Word:
                        res_add_mode = SpecialAddressingMode::AbsoluteShort;
                        break;

                    case DataType::Long:
                        res_add_mode = SpecialAddressingMode::AbsoluteLong;
                        break;
                    }

                    instr.operands[opNum].operandType =
                        OperandType::AbsoluteLong;
                    instr.operands[opNum].specialAddressingMode = res_add_mode;
                    instr.operands[opNum].value = utils::hash(parsed_str);
                    instr.operands[opNum].labelResolved = false;
                });

            return AnyOf(dec_mem, hex_mem, label_mem)(str);
        };
    }

    constexpr auto AnyRegister(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            auto register_parser = AnyOf(DataRegisterParser(instr, opNum),
                                         AddressRegisterParser(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AsAddress(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            auto register_parser = Map(
                Between(
                    Char('('), AddressRegisterParser(instr, opNum), Char(')')),
                [&](std::string_view parsed_str) {
                    instr.operands[opNum].operandType = OperandType::Address;
                });

            return register_parser(str);
        };
    }

    constexpr auto AddressWithDisplacement(momiji::Instruction& instr,
                                           int opNum)
    {
        // num(a*)
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            std::int32_t parsed_displacement = 0;

            auto num_parser =
                Map(GenericDecimal(), [&](std::string_view parsed_str) {
                    parsed_displacement =
                        std::stoll(std::string { parsed_str });
                });

            auto reg_parser =
                Map(AsAddress(instr, opNum), [&](std::string_view parsed_str) {
                    instr.operands[opNum].operandType =
                        OperandType::AddressOffset;
                });

            return Map(SeqNext(num_parser, reg_parser), [&](auto parsed_str) {
                instr.operands[opNum].value |= (parsed_displacement << 16);
            })(str);
        };
    }

    constexpr auto IndexedAddress(momiji::Instruction& instr, int opNum)
    {
        // (a*, *)
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            std::int16_t addreg = 0;
            std::int16_t othreg = 0;

            auto first_reg_parser =
                Map(AddressRegisterParser(instr, opNum),
                    [&](auto) { addreg = instr.operands[opNum].value; });

            auto oth_reg_parser = Map(AnyRegister(instr, opNum), [&](auto) {
                othreg = instr.operands[opNum].value;
                if (instr.operands[opNum].operandType ==
                    OperandType::AddressRegister)
                {
                    othreg += 8;
                }
            });

            auto combined_parser = SeqNext(AlwaysTrue(Whitespace()),
                                           first_reg_parser,
                                           AlwaysTrue(Whitespace()),
                                           Char(','),
                                           AlwaysTrue(Whitespace()),
                                           oth_reg_parser);

            return Map(
                Between(Char('('), combined_parser, Char(')')), [&](auto) {
                    instr.operands[opNum].operandType =
                        OperandType::AddressIndex;
                    std::int16_t higher         = (othreg << 12);
                    instr.operands[opNum].value = (higher << 16) | addreg;
                })(str);
        };
    }

    constexpr auto IndexedAddressWithDisplacement(momiji::Instruction& instr,
                                                  int opNum)
    {
        // (num, a*, *)
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            std::int16_t addreg = 0;
            std::int16_t othreg = 0;
            std::int16_t offset = 0;

            auto offset_parser = Map(GenericDecimal(), [&](auto parsed_str) {
                offset = std::stol(std::string { parsed_str });
            });

            auto first_reg_parser =
                Map(AddressRegisterParser(instr, opNum),
                    [&](auto) { addreg = instr.operands[opNum].value; });

            auto oth_reg_parser = Map(AnyRegister(instr, opNum), [&](auto) {
                othreg = instr.operands[opNum].value;
                if (instr.operands[opNum].operandType ==
                    OperandType::AddressRegister)
                {
                    othreg += 8;
                }
            });

            auto combined_parser = SeqNext(AlwaysTrue(Whitespace()),
                                           offset_parser,
                                           AlwaysTrue(Whitespace()),
                                           Char(','),
                                           AlwaysTrue(Whitespace()),
                                           first_reg_parser,
                                           AlwaysTrue(Whitespace()),
                                           Char(','),
                                           AlwaysTrue(Whitespace()),
                                           oth_reg_parser);

            return Map(
                Between(Char('('), combined_parser, Char(')')), [&](auto) {
                    instr.operands[opNum].operandType =
                        OperandType::AddressIndex;
                    std::int16_t higher         = offset | (othreg << 12);
                    instr.operands[opNum].value = (higher << 16) | addreg;
                })(str);
        };
    }

    constexpr auto AddressPreDecr(momiji::Instruction& instr, int opNum)
    {
        // -(a*)
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            auto combined_parser = SeqNext(
                Char('-'), AlwaysTrue(Whitespace()), AsAddress(instr, opNum));

            return Map(combined_parser, [&](auto) {
                instr.operands[opNum].operandType = OperandType::AddressPre;
            })(str);
        };
    }

    constexpr auto AddressPostIncr(momiji::Instruction& instr, int opNum)
    {
        // (a*)+
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            auto combined_parser = SeqNext(
                AsAddress(instr, opNum), AlwaysTrue(Whitespace()), Char('+'));

            return Map(combined_parser, [&](auto) {
                instr.operands[opNum].operandType = OperandType::AddressPost;
            })(str);
        };
    }

    constexpr auto AnyOperand(momiji::Instruction& instr, int opNum)
    {
        return [&instr, opNum](std::string_view str) -> parser_metadata {
            auto op_parser = AnyOf(OperandImmediate(instr, opNum),
                                   AnyRegister(instr, opNum),
                                   AddressPreDecr(instr, opNum),
                                   AddressPostIncr(instr, opNum),
                                   AddressWithDisplacement(instr, opNum),
                                   IndexedAddress(instr, opNum),
                                   IndexedAddressWithDisplacement(instr, opNum),
                                   AsAddress(instr, opNum),
                                   MemoryAddress(instr, opNum));

            return op_parser(str);
        };
    }

    constexpr auto ParseDataType(momiji::Instruction& instr)
    {
        return [&instr](std::string_view str) -> parser_metadata {
            constexpr auto inter_parser =
                Next(Char('.'), AnyOf(Char('b'), Char('w'), Char('l')));

            auto DataType_parser =
                Map(inter_parser, [&](std::string_view parsed_str) {
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

#if 0
    constexpr auto ResolveLabel(momiji::Instruction& instr,
            const LabelInfo& labels)
    {
        return [&instr, &labels](std::string_view str) -> parser_metadata {
            auto res = Word()(str);
            if (res.result)
            {
                const auto str_hash = utils::hash(res.parsed_str);

                auto it = alg::find_label(labels, str_hash);

                if (it != std::end(labels.labels))
                {
                    instr.operands[0].value = it->idx;
                    instr.operands[0].operandType = OperandType::AbsoluteLong;
                    instr.operands[0].labelResolved = true;
                }
                else
                {
                    instr.operands[0].value = str_hash;
                    instr.operands[0].labelResolved = false;
                }
            }

            return res;
        };
    }
#endif

    constexpr auto CommonInstructionParser(momiji::Instruction& instr)
    {
        return [&instr](std::string_view str) -> parser_metadata {
            auto parser = SeqNext(
                AlwaysTrue(ParseDataType(instr)), // data type is optional
                Whitespace(),
                AnyOperand(instr, 0),
                AlwaysTrue(Whitespace()),
                Char(','),
                AlwaysTrue(Whitespace()),
                AnyOperand(instr, 1));

            return parser(str);
        };
    }

    constexpr auto ImmediateInstructionParser(momiji::Instruction& instr)
    {
        return [&instr](std::string_view str) -> parser_metadata {
            auto parser = SeqNext(AlwaysTrue(ParseDataType(instr)),
                                  Whitespace(),
                                  OperandImmediate(instr, 0),
                                  AlwaysTrue(Whitespace()),
                                  Char(','),
                                  AlwaysTrue(Whitespace()),
                                  AnyOperand(instr, 1));

            return parser(str);
        };
    }

    constexpr auto BranchInstructionParser(momiji::Instruction& instr,
                                           const momiji::LabelInfo& labels)
    {
        return [&instr, &labels](std::string_view str) -> parser_metadata {
            auto parser = SeqNext(
                Whitespace(),
                AnyOf(OperandImmediate(instr, 0), MemoryAddress(instr, 0)));

            return parser(str);
        };
    }

    constexpr auto OneRegisterInstructionParser(momiji::Instruction& instr)
    {
        return [&instr](std::string_view str) -> parser_metadata {
            auto parser = SeqNext(AlwaysTrue(ParseDataType(instr)),
                                  Whitespace(),
                                  AnyRegister(instr, 0));

            return parser(str);
        };
    }

    constexpr auto ShiftInstructionParser(momiji::Instruction& instr)
    {
        return [&instr](std::string_view str) -> parser_metadata {
            auto tryRegMode = CommonInstructionParser(instr)(str);
            if (tryRegMode.result)
            {
                return tryRegMode;
            }

            auto tryMemMode = SeqNext(Whitespace(), AnyOperand(instr, 0));

            return tryMemMode(str);
        };
    }
} // namespace momiji
