#include "Instructions.h"

#include <cstdio>

namespace momiji::details
{
    namespace ops = momiji::operands;

    namespace
    {
        template <typename Operand>
        bool matchOp(const momiji::Operand& operand)
        {
            return std::holds_alternative<Operand>(operand);
        }

        bool checkRegRange(std::int32_t value)
        {
            return (value < 0) || (value > 7);
        }

        void sanitizeRegisters(const momiji::Operand& operand,
                               momiji::parser_metadata& metadata)
        {
            if (!metadata.result)
            {
                return;
            }

            auto setError = [&](const momiji::ParserError::ErrorType& error) {
                metadata.result = false;
                metadata.error  = std::move(error);
            };

            std::int8_t regval = momiji::extractRegister(operand);

            if (checkRegRange(regval))
            {
                setError(momiji::errors::InvalidRegisterNumber { regval });
            }
        }

        bool sanitizeAddressDataType(const momiji::ParsedInstruction& instr)
        {
            switch (instr.dataType)
            {
            case DataType::Byte:
                return false;

            default:
                return true;
            }
        }

        bool sanitizeImmediate(const momiji::Operand& op,
                               momiji::parser_metadata& metadata)
        {
            if (!metadata.result)
            {
                return false;
            }

            if (matchOp<ops::Immediate>(op))
            {
                momiji::errors::OperandTypeMismatch error {
                    {}, momiji::convertOperand(op)
                };

                metadata.result = false;
                metadata.error  = std::move(error);

                return false;
            }

            return true;
        }
    } // namespace

    momiji::parser_metadata parseMove(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = momiji::CommonInstructionParser(instr)(str);

        if (matchOp<ops::AddressRegister>(instr.operands[1]) &&
            !sanitizeAddressDataType(instr))
        {
            momiji::errors::DataTypeMismatch error { { momiji::DataType::Word,
                                                       momiji::DataType::Long },
                                                     instr.dataType };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (matchOp<ops::Immediate>(instr.operands[1]) ||
            matchOp<ops::Address>(instr.operands[1]))
        {
            momiji::errors::OperandTypeMismatch error {
                {
                    momiji::ParserOperand::DataRegister,
                    momiji::ParserOperand::AddressPost,
                    momiji::ParserOperand::AddressPre,
                    momiji::ParserOperand::AddressIndex,
                    momiji::ParserOperand::AddressOffset,
                    momiji::ParserOperand::AbsoluteShort,
                    momiji::ParserOperand::AbsoluteLong,
                },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.instructionType = InstructionType::Move;

        return res;
    }

    momiji::parser_metadata parseMoveQ(std::string_view str,
                                       momiji::ParsedInstruction& instr)
    {
        auto res = momiji::ImmediateInstructionParser(instr)(str);

        if (!matchOp<ops::Immediate>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::Immediate },
                momiji::convertOperand(instr.operands[0]),
                0
            };
            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (!matchOp<ops::DataRegister>(instr.operands[1]))
        {
            errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        sanitizeRegisters(instr.operands[1], res);

        instr.instructionType = InstructionType::MoveQuick;

        return res;
    }

    momiji::parser_metadata parseAdd(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto op1 = momiji::convertOperand(instr.operands[0]);
        const auto op2 = momiji::convertOperand(instr.operands[1]);

        switch (op2)
        {
        case ParserOperand::AddressRegister:
            instr.instructionType = InstructionType::AddA;

            if (!sanitizeAddressDataType(instr))
            {
                momiji::errors::DataTypeMismatch error {
                    { DataType::Word, DataType::Long }, instr.dataType
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            break;

        case ParserOperand::Immediate:
            sanitizeImmediate(instr.operands[1], res);
            break;

        default:
            instr.instructionType = InstructionType::Add;
        }

        switch (op1)
        {
        case ParserOperand::Immediate:
            instr.instructionType = InstructionType::AddI;
            break;

        default:
            break;
        }

        return res;
    }

    momiji::parser_metadata parseSub(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto op1 = momiji::convertOperand(instr.operands[0]);
        const auto op2 = momiji::convertOperand(instr.operands[1]);

        switch (op2)
        {
        case ParserOperand::AddressRegister:
            instr.instructionType = InstructionType::SubA;

            if (!sanitizeAddressDataType(instr))
            {
                momiji::errors::DataTypeMismatch error {
                    { DataType::Word, DataType::Long }, instr.dataType
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }
            break;

        case ParserOperand::Immediate:
            sanitizeImmediate(instr.operands[1], res);
            break;

        default:
            instr.instructionType = InstructionType::Sub;
        }

        switch (op1)
        {
        case ParserOperand::Immediate:
            instr.instructionType = InstructionType::SubI;
            break;

        default:
            break;
        }

        return res;
    }

    momiji::parser_metadata parseMuls(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        if (!matchOp<ops::DataRegister>(instr.operands[1]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (matchOp<operands::Address>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::AddressPost,
                  momiji::ParserOperand::AddressPre,
                  momiji::ParserOperand::AddressIndex,
                  momiji::ParserOperand::AddressOffset },
                momiji::convertOperand(instr.operands[0]),
                0
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::SignedMul;

        return res;
    }

    momiji::parser_metadata parseMulu(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        if (!matchOp<ops::DataRegister>(instr.operands[1]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (matchOp<operands::Address>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::AddressPost,
                  momiji::ParserOperand::AddressPre,
                  momiji::ParserOperand::AddressIndex,
                  momiji::ParserOperand::AddressOffset },
                momiji::convertOperand(instr.operands[0]),
                0
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::UnsignedMul;

        return res;
    }

    momiji::parser_metadata parseDivs(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        if (!matchOp<ops::DataRegister>(instr.operands[1]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (matchOp<operands::Address>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::AddressPost,
                  momiji::ParserOperand::AddressPre,
                  momiji::ParserOperand::AddressIndex,
                  momiji::ParserOperand::AddressOffset },
                momiji::convertOperand(instr.operands[0]),
                0
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::SignedDiv;

        return res;
    }

    momiji::parser_metadata parseDivu(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        if (!matchOp<ops::DataRegister>(instr.operands[1]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[1]),
                1
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        if (matchOp<operands::Address>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::AddressPost,
                  momiji::ParserOperand::AddressPre,
                  momiji::ParserOperand::AddressIndex,
                  momiji::ParserOperand::AddressOffset },
                momiji::convertOperand(instr.operands[0]),
                0
            };

            res.result = false;
            res.error  = std::move(error);

            return res;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::SignedDiv;

        return res;
    }

    momiji::parser_metadata parseSwap(std::string_view str,
                                      momiji::ParsedInstruction& instr)
    {
        auto res = OneRegisterInstructionParser(instr)(str);

        if (!matchOp<ops::DataRegister>(instr.operands[0]))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister },
                momiji::convertOperand(instr.operands[0]),
                0
            };
            res.result = false;
            res.error  = std::move(error);
        }

        sanitizeRegisters(instr.operands[0], res);

        instr.dataType        = DataType::Long;
        instr.instructionType = InstructionType::Swap;

        return res;
    }

    momiji::parser_metadata parseExg(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        auto invalidOp = [&](std::int8_t opNum) -> bool {
            const auto& op = instr.operands[opNum];

            return !matchOp<ops::DataRegister>(op) ||
                   !matchOp<ops::AddressRegister>(op);
        };

        for (std::int8_t i = 0; i < 2; ++i)
        {
            if (invalidOp(i))
            {
                momiji::errors::OperandTypeMismatch error {
                    { momiji::ParserOperand::DataRegister,
                      momiji::ParserOperand::AddressRegister },
                    momiji::convertOperand(instr.operands[i]),
                    i
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }
        }

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::Exchange;

        return res;
    }

    momiji::parser_metadata parseOr(std::string_view str,
                                    momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto& op1 = instr.operands[0];
        const auto& op2 = instr.operands[1];

        switch (convertOperand(op1))
        {
        case ParserOperand::Immediate:
            instr.instructionType = InstructionType::OrI;
            break;

        default:
            instr.instructionType = InstructionType::Or;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(op2, res);

        return res;
    }

    momiji::parser_metadata parseAnd(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto& op1 = instr.operands[0];
        const auto& op2 = instr.operands[1];

        switch (convertOperand(op1))
        {
        case ParserOperand::Immediate:
            instr.instructionType = InstructionType::AndI;
            break;

        default:
            instr.instructionType = InstructionType::And;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(op2, res);

        return res;
    }

    momiji::parser_metadata parseXor(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto& op1 = instr.operands[0];
        const auto& op2 = instr.operands[1];

        switch (convertOperand(op1))
        {
        case ParserOperand::Immediate:
            instr.instructionType = InstructionType::XorI;
            break;

        case ParserOperand::DataRegister:
            instr.instructionType = InstructionType::Xor;
            break;

        default:
            res.result = false;
            res.error  = momiji::errors::OperandTypeMismatch {};
            return res;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(op2, res);

        return res;
    }

    momiji::parser_metadata parseNot(std::string_view str,
                                     momiji::ParsedInstruction&)
    {
        return { false, str, "", {} };
    }

    momiji::parser_metadata parseCmp(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = CommonInstructionParser(instr)(str);

        const auto& op1 = instr.operands[0];
        const auto& op2 = instr.operands[1];

        if (!matchOp<ops::Immediate>(op1) ||
            (!matchOp<ops::DataRegister>(op2) &&
             !matchOp<ops::AddressRegister>(op2)))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::AddressRegister },
                convertOperand(op2),
                1
            };

            res.result = false;
            res.error  = std::move(error);
            return res;
        }

        if (matchOp<ops::Immediate>(op1) &&
            (matchOp<ops::AddressRegister>(op2) ||
             matchOp<ops::Immediate>(op2)))
        {
            momiji::errors::OperandTypeMismatch error {
                { momiji::ParserOperand::DataRegister,
                  momiji::ParserOperand::Address,
                  momiji::ParserOperand::AddressPre,
                  momiji::ParserOperand::AddressPost,
                  momiji::ParserOperand::AddressOffset,
                  momiji::ParserOperand::AddressIndex },
                convertOperand(op2),
                1
            };

            res.result = false;
            res.error  = std::move(error);
            return res;
        }

        switch (convertOperand(op2))
        {
        case momiji::ParserOperand::AddressRegister:
            instr.instructionType = InstructionType::CompareA;

            if (!sanitizeAddressDataType(instr))
            {
                momiji::errors::DataTypeMismatch error {
                    { DataType::Word, DataType::Long }, instr.dataType
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }
            break;

        default:
            instr.instructionType = InstructionType::Compare;
        }

        switch (convertOperand(op1))
        {
        case momiji::ParserOperand::Immediate:
            instr.instructionType = InstructionType::CompareI;
            break;
        default:
            break;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        return res;
    }

    momiji::parser_metadata parseTst(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::Tst;
        return OneRegisterInstructionParser(instr)(str);
    }

    momiji::parser_metadata parseJmp(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res =
            SeqNext(Whitespace(),
                    AnyOf(MemoryAddress(instr, 0), AsAddress(instr, 0)))(str);

        instr.instructionType = InstructionType::Jmp;

        const auto& op1 = instr.operands[0];
        const auto& op2 = instr.operands[1];

        switch (convertOperand(op1))
        {
        case ParserOperand::Address:
            sanitizeRegisters(op1, res);
            break;

        case ParserOperand::AbsoluteShort:
        case ParserOperand::AbsoluteLong:
            break;

        default:
            res.result = false;
            res.error  = errors::OperandTypeMismatch {};
        }

        return res;
    } // namespace v2

    momiji::parser_metadata parseJsr(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = parseJmp(str, instr);

        instr.instructionType = InstructionType::JmpSubroutine;

        return res;
    }

    momiji::parser_metadata parseBra(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::Branch;

        return res;
    }

    momiji::parser_metadata parseBsr(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = parseBra(str, instr);

        instr.instructionType = InstructionType::BranchSubroutine;

        return res;
    }

    momiji::parser_metadata parseRts(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::ReturnSubroutine;

        return { true, str, "", {} };
    }

    momiji::parser_metadata parseBlt(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessThan;

        return res;
    }

    momiji::parser_metadata parseBle(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessEq;

        return res;
    }

    momiji::parser_metadata parseBgt(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterThan;

        return res;
    }

    momiji::parser_metadata parseBge(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterEq;

        return res;
    }

    momiji::parser_metadata parseBeq(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::Equal;

        return res;
    }

    momiji::parser_metadata parseBne(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        auto res = BranchInstructionParser(instr)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::NotEqual;

        return res;
    }

    momiji::parser_metadata parseAsl(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::ArithmeticShiftLeft;

        return ShiftInstructionParser(instr)(str);
    }

    momiji::parser_metadata parseAsr(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::ArithmeticShiftRight;

        return ShiftInstructionParser(instr)(str);
    }

    momiji::parser_metadata parseLsl(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::LogicalShiftLeft;

        return ShiftInstructionParser(instr)(str);
    }

    momiji::parser_metadata parseLsr(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::LogicalShiftRight;

        return ShiftInstructionParser(instr)(str);
    }

    momiji::parser_metadata parseDeclare(std::string_view str,
                                         momiji::ParsedInstruction& instr)
    {
        parser_metadata res = (ParseDataType(instr))(str);

        instr.instructionType = InstructionType::Declare;

        auto singleOpParser = [](momiji::ParsedInstruction& instr,
                                 std::uint32_t opNum) {
            return [&instr, opNum](std::string_view str) -> parser_metadata {
                while (opNum >= instr.operands.size())
                {
                    instr.operands.emplace_back();
                }

                auto ret = MemoryAddress(instr, opNum);

                return ret(str);
            };
        };

        for (std::uint32_t i = 0; true; ++i)
        {
            auto tryNum = SeqNext(AlwaysTrue(Whitespace()),
                                  singleOpParser(instr, i))(res.rest);

            if (tryNum.result)
            {
                res = tryNum;
                auto tryComma =
                    SeqNext(AlwaysTrue(Whitespace()), Char(','))(res.rest);
                if (tryComma.result)
                {
                    res = tryComma;
                }
                else
                {
                    return res;
                }
            }
            else
            {
                return res;
            }
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunreachable-code-return"
        return res;
#pragma clang diagnostic pop
    }

    momiji::parser_metadata parseHcf(std::string_view str,
                                     momiji::ParsedInstruction& instr)
    {
        instr.instructionType = InstructionType::HaltCatchFire;

        instr.operands.resize(0);

        instr.dataType = DataType::Word;

        return { true, str, "", {} };
    }

} // namespace momiji::details
