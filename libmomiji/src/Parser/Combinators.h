#pragma once

#include <Parser.h>

#include <System.h>
#include <algorithm>
#include <string>
#include <string_view>

#include "Common.h"

namespace momiji
{
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
            bool done = false;

            parser_metadata res = { false, str, "", {} };

            (
                [&]() /*constexpr*/ {
                    if (done)
                    {
                        return;
                    }

                    res  = parsers(str);
                    done = !res.result;
                    str  = res.rest;
                }(),
                ...);

            return res;
        };
    }

    template <typename... Ts>
    constexpr auto AllSeqNext(Ts&&... parsers)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto savedStr = str;
            bool done     = false;

            std::int32_t idx = 0;

            parser_metadata res = { false, str, "", {} };

            (
                [&]() /*constexpr*/ {
                    if (done)
                    {
                        return;
                    }

                    res  = parsers(str);
                    done = !res.result;
                    str  = res.rest;
                    idx += res.parsed_str.size();
                }(),
                ...);

            res.parsed_str = savedStr.substr(0, idx);

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
                [&]() /*constexpr*/ {
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
                ret.parsed_str = backup.substr(0, std::uint32_t(idx));
            }

            return ret;
        };
    }

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

    template <typename Parser, typename Fun>
    constexpr auto MapFalse(Parser&& parser, Fun&& f)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res = parser(str);

            if (!res.result)
            {
                f(res.rest, res);
            }

            return res;
        };
    }

    template <typename Parser, typename Fun>
    constexpr auto MapMathAST(Parser&& parser, Fun&& f)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res = parser(str);

            if (res.first.result)
            {
                f(res.second);
            }

            return res.first;
        };
    }

    template <typename Parser, typename Fun>
    constexpr auto MapResult(Parser&& parser, Fun&& f)
    {
        return [=](std::string_view str) -> momiji::parser_metadata {
            auto res = parser(str);

            if (res.result)
            {
                f(res);
            }

            return res;
        };
    }

    template <typename Parser, typename Error>
    constexpr auto SetError(Parser&& parser, Error&& error)
    {
        return [=](std::string_view str) -> parser_metadata {
            return MapFalse(parser, [&](auto, parser_metadata& metadata) {
                auto& variant = metadata.error;
                if (std::holds_alternative<errors::UnknownError>(variant))
                {
                    variant = std::move(error);
                }
            })(str);
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
                    return { true, str.substr(cmpstr.size()), trim_str, {} };
                }
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto Char(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            if (!str.empty() && str[0] == c)
            {
                return { true, str.substr(1), str.substr(0, 1), {} };
            }

            return { false, str, "", errors::MissingCharacter { c } };
        };
    }

    constexpr auto NotChar(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            if (!str.empty() && str[0] != c)
            {
                return { true, str.substr(1), str.substr(0, 1), {} };
            }

            return { false, str, "", {} };
        };
    }

    constexpr auto ErroringChar(char c)
    {
        return [=](std::string_view str) -> parser_metadata {
            return MapFalse(Char(c),
                            [=](std::string_view, parser_metadata& metadata) {
                                metadata.error = errors::MissingCharacter { c };
                            })(str);
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
                    return { true, res2.rest, res1.parsed_str, {} };
                }
            }

            return { false, str, "", res1.error };
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

            return res1;
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

            return head_res;
        };
    }

    template <typename Parser>
    constexpr auto AlwaysTrue(Parser&& parser)
    {
        return [=](std::string_view str) -> parser_metadata {
            auto res = parser(str);
            return { true, res.rest, res.parsed_str, {} };
        };
    }

    constexpr auto Endl()
    {
        return [](std::string_view str) -> parser_metadata {
            if (str.empty())
            {
                return { true, str, "", {} };
            }

            return AnyOf(Char('\n'), Char('\0'), Char('\r'))(str);
        };
    }

    constexpr auto NotEndl()
    {
        return [](std::string_view str) -> parser_metadata {
            if (str.empty())
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
            }
        };
    }

    constexpr auto DecNumber()
    {
        return [](std::string_view str) -> parser_metadata {
            std::uint32_t idx = 0;
            bool found_num    = false;

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
            std::uint32_t idx = 0;

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
            if (!str.empty())
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

    inline math_parser_metadata MathFactor(std::string_view);
    inline math_parser_metadata MathExpression(std::string_view);

    inline math_parser_metadata MathFactor(std::string_view str)
    {
        auto decimal = GenericDecimal()(str);

        if (decimal.result)
        {
            momiji::objects::Number num { std::int32_t(
                std::stoll(std::string { decimal.parsed_str })) };

            return { decimal, make_node(num) };
        }

        auto hex = GenericHex()(str);

        if (hex.result)
        {
            momiji::objects::Number num { std::int32_t(
                std::stoll(std::string { hex.parsed_str }, nullptr, 16)) };

            return { hex, make_node(num) };
        }

        auto label = Word()(str);

        if (label.result)
        {
            momiji::objects::Label lbl { utils::hash(label.parsed_str) };

            return { label, make_node(lbl) };
        }

        std::unique_ptr<momiji::objects::MathASTNode> tryExprNode;

        auto tryExpr = Between(Char('('),
                               SeqNext(AlwaysTrue(Whitespace()),
                                       MapMathAST(MathExpression,
                                                  [&](auto& node) {
                                                      tryExprNode =
                                                          std::move(node);
                                                  }),
                                       AlwaysTrue(Whitespace())),
                               Char(')'))(str);

        return { tryExpr, std::move(tryExprNode) };
    }

    // factor * factor
    // factor / factor
    // factor
    inline math_parser_metadata MathTerm(std::string_view str)
    {
        auto factor = MathFactor(str);

        if (!factor.first.result)
        {
            return factor;
        }

        std::unique_ptr<momiji::objects::MathASTNode> rightNode;

        auto matchFn = [&](char op) {
            return SeqNext(AlwaysTrue(Whitespace()),
                           Char(op),
                           AlwaysTrue(Whitespace()),
                           MapMathAST(MathTerm, [&](auto& node) {
                               rightNode = std::move(node);
                           }))(factor.first.rest);
        };

        auto tryMul = matchFn('*');

        if (tryMul.result)
        {
            momiji::objects::MathOperator mulOp;
            mulOp.left  = std::move(factor.second);
            mulOp.right = std::move(rightNode);
            mulOp.type  = momiji::objects::MathOperator::Type::Mul;

            return { tryMul, make_node(mulOp) };
        }

        auto tryDiv = matchFn('/');

        if (tryDiv.result)
        {
            momiji::objects::MathOperator divOp;
            divOp.left  = std::move(factor.second);
            divOp.right = std::move(rightNode);
            divOp.type  = momiji::objects::MathOperator::Type::Div;

            return { tryDiv, make_node(divOp) };
        }

        return factor;
    }

    // term + term
    // term - term
    // term
    inline math_parser_metadata MathExpression(std::string_view str)
    {
        auto term = MathTerm(str);

        if (!term.first.result)
        {
            return term;
        }

        std::unique_ptr<momiji::objects::MathASTNode> rightNode;

        auto matchFn = [&](char op) {
            return SeqNext(AlwaysTrue(Whitespace()),
                           Char(op),
                           AlwaysTrue(Whitespace()),
                           MapMathAST(MathExpression, [&](auto& node) {
                               rightNode = std::move(node);
                           }))(term.first.rest);
        };

        auto tryPlus = matchFn('+');

        if (tryPlus.result)
        {
            momiji::objects::MathOperator addOp;
            addOp.left  = std::move(term.second);
            addOp.right = std::move(rightNode);
            addOp.type  = momiji::objects::MathOperator::Type::Add;

            return { tryPlus, make_node(addOp) };
        }

        auto tryMinus = matchFn('-');

        if (tryMinus.result)
        {
            momiji::objects::MathOperator subOp;
            subOp.left  = std::move(term.second);
            subOp.right = std::move(rightNode);
            subOp.type  = momiji::objects::MathOperator::Type::Sub;

            return { tryMinus, make_node(subOp) };
        }

        return term;
    }

    inline auto
    MathExpressionMain(std::unique_ptr<momiji::objects::MathASTNode>& output)
    {
        return [&](std::string_view str) -> parser_metadata {
            auto res = MathExpression(str);

            output = std::move(res.second);

            return res.first;
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

    constexpr auto ParseDirective()
    {
        return [](std::string_view str) -> parser_metadata {
            return AllSeqNext(Char('.'), Word())(str);
        };
    }

    constexpr auto InstructionWord()
    {
        return [](std::string_view str) -> parser_metadata {
            return AnyOf(Word(), ParseDirective())(str);
        };
    }

    inline auto OperandImmediate(momiji::ParsedInstruction& instr,
                                 std::uint32_t opNum)
    {
        return
            [&instr, opNum](std::string_view str) -> momiji::parser_metadata {
                constexpr auto inter_dec_parser =
                    SeqNext(Char('#'), GenericDecimal());
#if 0

            auto decimal_num =
                Map(inter_dec_parser, [&instr, opNum](auto parsed_str) {
                    const std::int64_t val =
                        std::stoll(std::string { parsed_str });

                    momiji::operands::Immediate op;
                    op.value = std::make_unique<momiji::objects::MathASTNode>();
                    op.value->value =
                        momiji::objects::Number { std::int32_t(val) };

                    instr.operands[opNum] = std::move(op);
                });

            constexpr auto inter_hex_parser = SeqNext(Char('#'), GenericHex());
            auto hex_num                    = Map(
                inter_hex_parser, [&instr, opNum](std::string_view parsed_str) {
                    const std::int64_t val =
                        std::stoll(std::string { parsed_str }, nullptr, 16);

                    momiji::operands::Immediate op;
                    op.value = std::make_unique<momiji::objects::MathASTNode>();
                    op.value->value =
                        momiji::objects::Number { std::int32_t(val) };

                    instr.operands[opNum] = std::move(op);
                });

            // Example: move.w #arr, a0
            //          ^ moves the "address" of "arr" in a0
            constexpr auto inter_imm_label_parser = SeqNext(Char('#'), Word());
            auto imm_label =
                Map(inter_imm_label_parser, [&instr, opNum](auto parsed_str) {
                    const auto hash = utils::hash(parsed_str);

                    momiji::operands::Immediate op;
                    op.value = std::make_unique<momiji::objects::MathASTNode>();
                    op.value->value = momiji::objects::Label { hash };

                    instr.operands[opNum] = std::move(op);
                });


            return AnyOf(decimal_num, hex_num, imm_label)(str);
#else
                std::unique_ptr<momiji::objects::MathASTNode> root;

                return Map(SeqNext(Char('#'),
                                   AlwaysTrue(Whitespace()),
                                   MathExpressionMain(root)),
                           [&](auto&) {
                               momiji::operands::Immediate op;
                               op.value              = std::move(root);
                               instr.operands[opNum] = std::move(op);
                           })(str);
#endif
            };
    }

    constexpr auto AddressRegisterParser(momiji::ParsedInstruction& instr,
                                         std::uint32_t opNum)
    {
        return
            [&instr, opNum](std::string_view str) -> momiji::parser_metadata {
                constexpr auto inter_parser = SeqNext(Char('a'), DecNumber());

                bool rangeValid = true;

                auto register_parser =
                    MapResult(inter_parser, [&](auto& resultRef) {
                        const auto reg_num =
                            std::stoll(std::string { resultRef.parsed_str });

                        instr.operands[opNum] =
                            operands::AddressRegister { std::int32_t(reg_num) };
                    });

                return SetError(register_parser,
                                errors::MissingCharacter { 'a' })(str);
            };
    }

    constexpr auto DataRegisterParser(momiji::ParsedInstruction& instr,
                                      std::uint32_t opNum)
    {
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            constexpr auto inter_parser = SeqNext(Char('d'), DecNumber());
            auto register_parser =
                Map(inter_parser, [&](std::string_view parsed_str) {
                    const auto reg_num = std::stoll(std::string { parsed_str });

                    instr.operands[opNum] =
                        operands::DataRegister { std::int32_t(reg_num) };
                });

            return register_parser(str);
        };
    }

    inline auto MemoryAddress(momiji::ParsedInstruction& instr,
                              std::uint32_t opNum)
    {
        return
            [&instr, opNum](std::string_view str) -> momiji::parser_metadata {
                std::unique_ptr<momiji::objects::MathASTNode> root;

                switch (instr.dataType)
                {
                case DataType::Byte:
                case DataType::Word:
                    return Map(MathExpressionMain(root), [&](auto&) {
                        momiji::operands::AbsoluteShort op;
                        op.value              = std::move(root);
                        instr.operands[opNum] = std::move(op);
                    })(str);

                case DataType::Long:
                    return Map(MathExpressionMain(root), [&](auto&) {
                        momiji::operands::AbsoluteLong op;
                        op.value              = std::move(root);
                        instr.operands[opNum] = std::move(op);
                    })(str);
                }

                return { false, str, "", {} };
            };
    }

    constexpr auto AnyRegister(momiji::ParsedInstruction& instr,
                               std::uint32_t opNum)
    {
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            auto register_parser = AnyOf(DataRegisterParser(instr, opNum),
                                         AddressRegisterParser(instr, opNum));

            return register_parser(str);
        };
    }

    constexpr auto AsAddress(momiji::ParsedInstruction& instr,
                             std::uint32_t opNum)
    {
        return
            [&instr, opNum](std::string_view str) -> momiji::parser_metadata {
                auto register_parser =
                    Map(Between(Char('('),
                                AddressRegisterParser(instr, opNum),
                                Char(')')),
                        [&](std::string_view /*parsed_str*/) {
                            auto& op = instr.operands[opNum];

                            momiji::operands::Address tmp;
                            tmp.reg = extractRegister(op);

                            op = std::move(tmp);
                        });

                return register_parser(str);
            };
    }

    constexpr auto AddressWithDisplacement(momiji::ParsedInstruction& instr,
                                           std::uint32_t opNum)
    {
        // num(a*)
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            std::int32_t parsed_displacement = 0;

            auto num_parser =
                Map(GenericDecimal(), [&](std::string_view parsed_str) {
                    parsed_displacement =
                        std::int32_t(std::stoll(std::string { parsed_str }));
                });

            auto reg_parser =
                Map(AsAddress(instr, opNum), [&](auto /*parsed_str*/) {
                    auto& op = instr.operands[opNum];

                    momiji::operands::Address tmp;
                    tmp.reg = extractRegister(op);
                    op      = std::move(tmp);
                });

            return Map(SeqNext(num_parser, reg_parser),
                       [&](auto /*parsed_str*/) {
                           auto& op = std::get<momiji::operands::Address>(
                               instr.operands[opNum]);

                           momiji::operands::AddressOffset tmp;
                           tmp.reg = op.reg;

                           tmp.offset =
                               std::make_unique<momiji::objects::MathASTNode>();
                           tmp.offset->value =
                               momiji::objects::Number { parsed_displacement };

                           instr.operands[opNum] = std::move(tmp);
                       })(str);
        };
    }

    constexpr auto IndexedAddress(momiji::ParsedInstruction& instr,
                                  std::uint32_t opNum)
    {
        // (a*, *)
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            std::int32_t addreg = 0;
            std::int32_t othreg = 0;

            auto first_reg_parser =
                Map(AddressRegisterParser(instr, opNum), [&](auto) {
                    addreg = extractRegister(instr.operands[opNum]);
                });

            auto oth_reg_parser = Map(AnyRegister(instr, opNum), [&](auto) {
                auto& op = instr.operands[opNum];
                othreg   = extractRegister(instr.operands[opNum]);

                if (std::holds_alternative<operands::AddressRegister>(op))
                {
                    othreg += 8;
                }
            });

            auto combined_parser = SeqNext(AlwaysTrue(Whitespace()),
                                           first_reg_parser,
                                           AlwaysTrue(Whitespace()),
                                           ErroringChar(','),
                                           AlwaysTrue(Whitespace()),
                                           oth_reg_parser);

            return Map(
                Between(ErroringChar('('), combined_parser, ErroringChar(')')),
                [&](auto) {
                    momiji::operands::AddressIndex tmp;
                    tmp.reg    = addreg;
                    tmp.othreg = othreg;

                    instr.operands[opNum] = std::move(tmp);
                })(str);
        };
    }

    constexpr auto
    IndexedAddressWithDisplacement(momiji::ParsedInstruction& instr,
                                   std::uint32_t opNum)
    {
        // (num, a*, *)
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            std::int32_t addreg = 0;
            std::int32_t othreg = 0;
            std::unique_ptr<momiji::objects::MathASTNode> offset;

            auto offset_parser = Map(GenericDecimal(), [&](auto parsed_str) {
                const auto val =
                    std::stoll(std::string { parsed_str }, nullptr, 16);

                offset = std::make_unique<momiji::objects::MathASTNode>();
                offset->value = momiji::objects::Number { std::int32_t(val) };
            });

            auto first_reg_parser =
                Map(AddressRegisterParser(instr, opNum), [&](auto) {
                    addreg = extractRegister(instr.operands[opNum]);
                });

            auto oth_reg_parser = Map(AnyRegister(instr, opNum), [&](auto) {
                auto& op = instr.operands[opNum];
                othreg   = extractRegister(op);
                if (std::holds_alternative<operands::AddressRegister>(op))
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

            return Map(Between(Char('('), combined_parser, Char(')')),
                       [&](auto) {
                           momiji::operands::AddressIndex tmp;

                           tmp.reg    = addreg;
                           tmp.othreg = othreg;
                           tmp.offset = std::move(offset);

                           instr.operands[opNum] = std::move(tmp);
                       })(str);
        };
    }

    constexpr auto AddressPreDecr(momiji::ParsedInstruction& instr,
                                  std::uint32_t opNum)
    {
        // -(a*)
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            auto combined_parser = SeqNext(
                Char('-'), AlwaysTrue(Whitespace()), AsAddress(instr, opNum));

            return Map(combined_parser, [&](auto) {
                auto& op = instr.operands[opNum];

                momiji::operands::AddressPre tmp;

                tmp.reg = extractRegister(op);

                op = tmp;
            })(str);
        };
    }

    constexpr auto AddressPostIncr(momiji::ParsedInstruction& instr,
                                   std::uint32_t opNum)
    {
        // (a*)+
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            auto combined_parser = SeqNext(
                AsAddress(instr, opNum), AlwaysTrue(Whitespace()), Char('+'));

            return Map(combined_parser, [&](auto) {
                auto& op = instr.operands[opNum];

                momiji::operands::AddressPost tmp;

                tmp.reg = extractRegister(op);

                op = tmp;
            })(str);
        };
    }

    constexpr auto AnyOperand(momiji::ParsedInstruction& instr,
                              std::uint32_t opNum)
    {
        return [&instr,
                opNum](std::string_view str) -> momiji::parser_metadata {
            auto op_parser = AnyOf(OperandImmediate(instr, opNum),
                                   AnyRegister(instr, opNum),
                                   AddressPreDecr(instr, opNum),
                                   AddressPostIncr(instr, opNum),
                                   AddressWithDisplacement(instr, opNum),
                                   IndexedAddress(instr, opNum),
                                   IndexedAddressWithDisplacement(instr, opNum),
                                   AsAddress(instr, opNum),
                                   MemoryAddress(instr, opNum));

            return SetError(op_parser, errors::UnknownOperand {})(str);
        };
    }

    constexpr auto ParseDataType(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
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

    constexpr auto CommonInstructionParser(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
            bool signal_error = false;
            ParserError::ErrorType error;

            auto parser = SeqNext(
                AlwaysTrue(ParseDataType(instr)), // data type is optional
                Whitespace(),
                momiji::AnyOperand(instr, 0),
                AlwaysTrue(Whitespace()),
                Char(','),
                AlwaysTrue(Whitespace()),
                momiji::AnyOperand(instr, 1));

            auto res = parser(str);

            if (signal_error)
            {
                res.error = std::move(error);
            }

            return res;
        };
    }

    constexpr auto ImmediateInstructionParser(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
            auto parser = SeqNext(AlwaysTrue(ParseDataType(instr)),
                                  Whitespace(),
                                  momiji::OperandImmediate(instr, 0),
                                  AlwaysTrue(Whitespace()),
                                  ErroringChar(','),
                                  AlwaysTrue(Whitespace()),
                                  momiji::AnyOperand(instr, 1));

            return parser(str);
        };
    }

    constexpr auto BranchInstructionParser(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
            auto parser = SeqNext(Whitespace(),
                                  AnyOf(momiji::OperandImmediate(instr, 0),
                                        momiji::MemoryAddress(instr, 0)));

            errors::OperandTypeMismatch error;

            return SetError(parser, std::move(error))(str);
        };
    }

    constexpr auto
    OneRegisterInstructionParser(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
            auto parser = SeqNext(AlwaysTrue(ParseDataType(instr)),
                                  Whitespace(),
                                  momiji::AnyRegister(instr, 0));

            return parser(str);
        };
    }

    constexpr auto ShiftInstructionParser(momiji::ParsedInstruction& instr)
    {
        return [&instr](std::string_view str) -> momiji::parser_metadata {
            auto isImmediate = [](const momiji::Operand& op) -> bool {
                return std::holds_alternative<momiji::operands::Immediate>(op);
            };

            auto tryRegMode = CommonInstructionParser(instr)(str);
            if (tryRegMode.result)
            {
                if (!std::holds_alternative<operands::DataRegister>(
                        instr.operands[1]))
                {
                    momiji::errors::OperandTypeMismatch error {
                        { momiji::ParserOperand::DataRegister },
                        momiji::convertOperand(instr.operands[1]),
                        1
                    };

                    tryRegMode.result = false;
                    tryRegMode.error  = std::move(error);
                }
                else if (!isImmediate(instr.operands[0]) &&
                         !std::holds_alternative<operands::DataRegister>(
                             instr.operands[0]))
                {
                    errors::OperandTypeMismatch error {
                        { momiji::ParserOperand::DataRegister,
                          momiji::ParserOperand::Immediate },
                        momiji::convertOperand(instr.operands[0]),
                        0
                    };

                    tryRegMode.result = false;
                    tryRegMode.error  = std::move(error);
                }

                return tryRegMode;
            }

            auto tryMemMode =
                SeqNext(Whitespace(), momiji::AnyOperand(instr, 0));

            return tryMemMode(str);
        };
    }
} // namespace momiji
