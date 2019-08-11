#include "Instructions.h"

#include <cstdio>

namespace momiji::details
{
#if 0
    namespace v1
    {
        static bool checkRegRange(std::int32_t value)
        {
            return (value < 0) || (value > 7);
        }

        static bool isImmediate(const momiji::v1::Operand& op)
        {
            return op.operandType == OperandType::Immediate &&
                   op.specialAddressingMode == SpecialAddressingMode::Immediate;
        }

        static void sanitizeRegisters(const momiji::v1::Operand& op,
                                      parser_metadata& metadata)
        {
            if (!metadata.result)
            {
                return;
            }

            auto setError = [&](const ParserError::ErrorType& error) {
                metadata.result = false;
                metadata.error  = std::move(error);
            };

            std::int16_t value = 0;

            switch (op.operandType)
            {
            case OperandType::DataRegister:
                [[fallthrough]];
            case OperandType::AddressRegister:
                [[fallthrough]];
            case OperandType::Address:
            case OperandType::AddressPre:
            case OperandType::AddressPost:
                value = std::int16_t(op.value);
                break;

            case OperandType::AddressOffset:
            case OperandType::AddressIndex:
                value = std::int16_t(op.value & 0x0000FFFF);
                break;

            default:
                return;
            }

            if (checkRegRange(value))
            {
                setError(errors::InvalidRegisterNumber { value });
            }
        }

        static bool sanitizeImmediate(const momiji::v1::Operand& op,
                                      parser_metadata& metadata)
        {
            if (!metadata.result)
            {
                return false;
            }

            if (isImmediate(op))
            {
                errors::OperandTypeMismatch error { {}, op.operandType };

                metadata.result = false;
                metadata.error  = std::move(error);

                return false;
            }

            return true;
        }

        parser_metadata parseMove(std::string_view str,
                                  momiji::v1::Instruction& instr,
                                  LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (isImmediate(instr.operands[1]) ||
                (instr.operands[1].operandType == OperandType::Address))
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::AddressPost,
                      OperandType::AddressPre,
                      OperandType::AddressIndex,
                      OperandType::AddressOffset },
                    instr.operands[1].operandType,
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

        parser_metadata
        parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = ImmediateInstructionParser(instr)(str);

            if (!isImmediate(instr.operands[0]))
            {
                errors::OperandTypeMismatch error {
                    { OperandType::Immediate }, instr.operands[0].operandType, 0
                };
                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (instr.operands[1].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[1].operandType,
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

        parser_metadata
        parseAdd(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            switch (instr.operands[1].operandType)
            {
            case OperandType::AddressRegister:
                instr.instructionType = InstructionType::AddA;
                break;

            case OperandType::Immediate:
                sanitizeImmediate(instr.operands[1], res);
                break;

            default:
                instr.instructionType = InstructionType::Add;
            }

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                switch (instr.operands[0].specialAddressingMode)
                {
                case SpecialAddressingMode::Immediate:
                    instr.instructionType = InstructionType::AddI;
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }

            return res;
        }

        parser_metadata
        parseSub(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            instr.instructionType = InstructionType::Sub;
            switch (instr.operands[1].operandType)
            {
            case OperandType::AddressRegister:
                instr.instructionType = InstructionType::SubA;
                break;

            case OperandType::Immediate:
                sanitizeImmediate(instr.operands[1], res);
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

                default:
                    break;
                }
                break;

            default:
                break;
            }

            for (const auto& op : instr.operands)
            {
                sanitizeRegisters(op, res);
            }

            return res;
        }

        parser_metadata
        parseMuls(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (instr.operands[1].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (instr.operands[0].operandType == OperandType::Address)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::AddressPost,
                      OperandType::AddressPre,
                      OperandType::AddressIndex,
                      OperandType::AddressOffset },
                    instr.operands[0].operandType,
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

        parser_metadata
        parseMulu(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (instr.operands[1].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (instr.operands[0].operandType == OperandType::Address)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::AddressPost,
                      OperandType::AddressPre,
                      OperandType::AddressIndex,
                      OperandType::AddressOffset },
                    instr.operands[0].operandType,
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

            instr.dataType        = DataType::Word;
            instr.instructionType = InstructionType::UnsignedMul;

            return res;
        }

        parser_metadata
        parseDivs(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (instr.operands[1].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (instr.operands[0].operandType == OperandType::Address)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::AddressPost,
                      OperandType::AddressPre,
                      OperandType::AddressIndex,
                      OperandType::AddressOffset },
                    instr.operands[0].operandType,
                    0
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            instr.dataType        = DataType::Word;
            instr.instructionType = InstructionType::SignedDiv;

            return res;
        }

        parser_metadata
        parseDivu(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (instr.operands[1].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (instr.operands[0].operandType == OperandType::Address)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::AddressPost,
                      OperandType::AddressPre,
                      OperandType::AddressIndex,
                      OperandType::AddressOffset },
                    instr.operands[0].operandType,
                    0
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            instr.dataType        = DataType::Word;
            instr.instructionType = InstructionType::UnsignedDiv;

            return res;
        }

        parser_metadata
        parseSwap(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = OneRegisterInstructionParser(instr)(str);

            if (instr.operands[0].operandType != OperandType::DataRegister)
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister },
                    instr.operands[0].operandType,
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

        parser_metadata
        parseExg(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            auto invalidOp = [&](std::int8_t opNum) -> bool {
                return (asl::saccess(instr.operands, opNum).operandType !=
                        OperandType::DataRegister) ||
                       (asl::saccess(instr.operands, opNum).operandType !=
                        OperandType::AddressRegister);
            };

            for (std::int8_t i = 0; i < 2; ++i)
            {
                if (invalidOp(i))
                {
                    errors::OperandTypeMismatch error {
                        { OperandType::DataRegister,
                          OperandType::AddressRegister },
                        asl::saccess(instr.operands, i).operandType,
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

        parser_metadata
        parseOr(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                switch (instr.operands[0].specialAddressingMode)
                {
                case SpecialAddressingMode::Immediate:
                    instr.instructionType = InstructionType::OrI;
                    break;

                default:
                    break;
                }
                break;

            default:
                instr.instructionType = InstructionType::Or;
            }

            for (const auto& op : instr.operands)
            {
                sanitizeRegisters(op, res);
            }

            sanitizeImmediate(instr.operands[1], res);

            return res;
        }

        parser_metadata
        parseAnd(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                switch (instr.operands[0].specialAddressingMode)
                {
                case SpecialAddressingMode::Immediate:
                    instr.instructionType = InstructionType::AndI;
                    break;

                default:
                    break;
                }
                break;

            default:
                instr.instructionType = InstructionType::And;
            }

            for (const auto& op : instr.operands)
            {
                sanitizeRegisters(op, res);
            }

            sanitizeImmediate(instr.operands[1], res);

            return res;
        }

        parser_metadata
        parseNot(std::string_view str, Instruction& /*instr*/, LabelInfo&)
        {
            return { false, str, "", {} };
        }

        parser_metadata
        parseXor(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                switch (instr.operands[0].specialAddressingMode)
                {
                case SpecialAddressingMode::Immediate:
                    instr.instructionType = InstructionType::XorI;
                    break;

                default:
                    break;
                }
                break;

            default:
                instr.instructionType = InstructionType::Xor;
            }

            for (const auto& op : instr.operands)
            {
                sanitizeRegisters(op, res);
            }

            sanitizeImmediate(instr.operands[1], res);

            if (instr.operands[0].operandType != OperandType::DataRegister)
            {
                res.result = false;
                res.error  = errors::OperandTypeMismatch {};
            }

            return res;
        }

        parser_metadata
        parseCmp(std::string_view str, Instruction& instr, LabelInfo&)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (!isImmediate(instr.operands[0]) ||
                (instr.operands[1].operandType != OperandType::DataRegister) &&
                (instr.operands[1].operandType != OperandType::AddressRegister))
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister, OperandType::AddressRegister },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);
                return res;
            }

            if (isImmediate(instr.operands[0]) &&
                ((instr.operands[1].operandType ==
                  OperandType::AddressRegister) ||
                 isImmediate(instr.operands[1])))
            {
                errors::OperandTypeMismatch error {
                    { OperandType::DataRegister,
                      OperandType::Address,
                      OperandType::AddressPre,
                      OperandType::AddressPost,
                      OperandType::AddressOffset,
                      OperandType::AddressIndex },
                    instr.operands[1].operandType,
                    1
                };

                res.result = false;
                res.error  = std::move(error);
                return res;
            }

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

                default:
                    break;
                }
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

        parser_metadata
        parseTst(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType = InstructionType::Tst;
            return OneRegisterInstructionParser(instr)(str);
        }

        parser_metadata parseJmp(std::string_view str,
                                 Instruction& instr,
                                 LabelInfo& /*labels*/)
        {
            auto res = SeqNext(
                Whitespace(),
                AnyOf(MemoryAddress(instr, 0), AsAddress(instr, 0)))(str);

            instr.instructionType = InstructionType::Jmp;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Address:
                sanitizeRegisters(instr.operands[0], res);
                break;

            case OperandType::AbsoluteLong:
                switch (instr.operands[0].specialAddressingMode)
                {
                case SpecialAddressingMode::AbsoluteLong:
                case SpecialAddressingMode::AbsoluteShort:
                    break;

                default:
                    res.result = false;
                    res.error  = errors::OperandTypeMismatch {};
                }
                break;

            default:
                res.result = false;
                res.error  = errors::OperandTypeMismatch {};
            }

            return res;
        }

        parser_metadata
        parseJsr(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = parseJmp(str, instr, labels);

            instr.instructionType = InstructionType::JmpSubroutine;

            return res;
        }

        parser_metadata
        parseBra(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::Branch;

            return res;
        }

        parser_metadata
        parseBsr(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = parseBra(str, instr, labels);

            instr.instructionType = InstructionType::BranchSubroutine;

            return res;
        }

        parser_metadata
        parseRts(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType         = InstructionType::ReturnSubroutine;
            instr.operands[0].operandType = OperandType::DataRegister;
            instr.operands[1].operandType = OperandType::DataRegister;

            return { true, str, "", {} };
        }

        parser_metadata
        parseBlt(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::LessThan;

            return res;
        }

        parser_metadata
        parseBle(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::LessEq;

            return res;
        }

        parser_metadata
        parseBgt(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::GreaterThan;

            return res;
        }

        parser_metadata
        parseBge(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::GreaterEq;

            return res;
        }

        parser_metadata
        parseBeq(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::Equal;

            return res;
        }

        parser_metadata
        parseBne(std::string_view str, Instruction& instr, LabelInfo& labels)
        {
            auto res = BranchInstructionParser(instr, labels)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::NotEqual;

            return res;
        }

        parser_metadata
        parseAsl(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType = InstructionType::ArithmeticShiftLeft;

            return ShiftInstructionParser(instr)(str);
        }

        parser_metadata
        parseAsr(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType = InstructionType::ArithmeticShiftRight;

            return ShiftInstructionParser(instr)(str);
        }

        parser_metadata
        parseLsl(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType = InstructionType::LogicalShiftLeft;

            return ShiftInstructionParser(instr)(str);
        }

        parser_metadata
        parseLsr(std::string_view str, Instruction& instr, LabelInfo&)
        {
            instr.instructionType = InstructionType::LogicalShiftRight;

            return ShiftInstructionParser(instr)(str);
        }

        parser_metadata parseDeclare(std::string_view str,
                                     Instruction& instr,
                                     LabelInfo& /*labels*/)
        {
            parser_metadata res = (ParseDataType(instr))(str);

            instr.instructionType = InstructionType::Declare;

            auto singleOpParser = [](Instruction& instr, std::uint32_t opNum) {
                return
                    [&instr, opNum](std::string_view str) -> parser_metadata {
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

        parser_metadata parseHcf(std::string_view str,
                                 Instruction& instr,
                                 LabelInfo& /*labels*/)
        {
            instr.instructionType = InstructionType::HaltCatchFire;

            instr.operands[0].operandType = OperandType::Immediate;
            instr.operands[0].specialAddressingMode =
                SpecialAddressingMode::Immediate;

            instr.operands[1].operandType = OperandType::DataRegister;

            instr.dataType = DataType::Word;

            return { true, str, "", {} };
        }
    } // namespace v1

#endif

    inline namespace v2
    {
        namespace ops = momiji::v2::operands;

        namespace
        {
            template <typename Operand>
            bool matchOp(const momiji::v2::Operand& operand)
            {
                return std::holds_alternative<Operand>(operand);
            }

            bool checkRegRange(std::int32_t value)
            {
                return (value < 0) || (value > 7);
            }

            void sanitizeRegisters(const momiji::v2::Operand& operand,
                                   momiji::v2::parser_metadata& metadata)
            {
                if (!metadata.result)
                {
                    return;
                }

                auto setError =
                    [&](const momiji::v2::ParserError::ErrorType& error) {
                        metadata.result = false;
                        metadata.error  = std::move(error);
                    };

                std::int8_t regval = momiji::v2::extractRegister(operand);

                if (v2::checkRegRange(regval))
                {
                    setError(
                        momiji::v2::errors::InvalidRegisterNumber { regval });
                }
            }

            bool sanitizeImmediate(const momiji::v2::Operand& op,
                                   momiji::v2::parser_metadata& metadata)
            {
                if (!metadata.result)
                {
                    return false;
                }

                if (matchOp<ops::Immediate>(op))
                {
                    momiji::v2::errors::OperandTypeMismatch error {
                        {}, momiji::v2::convertOperand(op)
                    };

                    metadata.result = false;
                    metadata.error  = std::move(error);

                    return false;
                }

                return true;
            }
        } // namespace

        momiji::v2::parser_metadata
        parseMove(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = momiji::v2::CommonInstructionParser(instr)(str);

            if (matchOp<ops::Immediate>(instr.operands[1]) ||
                matchOp<ops::Address>(instr.operands[1]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressIndex,
                      momiji::v2::ParserOperand::AddressOffset },
                    momiji::v2::convertOperand(instr.operands[1]),
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

        momiji::v2::parser_metadata
        parseMoveQ(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = momiji::v2::ImmediateInstructionParser(instr)(str);

            if (!matchOp<ops::Immediate>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::Immediate },
                    momiji::v2::convertOperand(instr.operands[0]),
                    0
                };
                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (!matchOp<ops::DataRegister>(instr.operands[1]))
            {
                errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[1]),
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

        momiji::v2::parser_metadata
        parseAdd(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            const auto op1 = momiji::v2::convertOperand(instr.operands[0]);
            const auto op2 = momiji::v2::convertOperand(instr.operands[1]);

            switch (op2)
            {
            case ParserOperand::AddressRegister:
                instr.instructionType = InstructionType::AddA;
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

        momiji::v2::parser_metadata
        parseSub(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            const auto op1 = momiji::v2::convertOperand(instr.operands[0]);
            const auto op2 = momiji::v2::convertOperand(instr.operands[1]);

            switch (op2)
            {
            case ParserOperand::AddressRegister:
                instr.instructionType = InstructionType::SubA;
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

        momiji::v2::parser_metadata
        parseMuls(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (!matchOp<ops::DataRegister>(instr.operands[1]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[1]),
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (matchOp<operands::Address>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressIndex,
                      momiji::v2::ParserOperand::AddressOffset },
                    momiji::v2::convertOperand(instr.operands[0]),
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

        momiji::v2::parser_metadata
        parseMulu(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (!matchOp<ops::DataRegister>(instr.operands[1]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[1]),
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (matchOp<operands::Address>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressIndex,
                      momiji::v2::ParserOperand::AddressOffset },
                    momiji::v2::convertOperand(instr.operands[0]),
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

        momiji::v2::parser_metadata
        parseDivs(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (!matchOp<ops::DataRegister>(instr.operands[1]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[1]),
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (matchOp<operands::Address>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressIndex,
                      momiji::v2::ParserOperand::AddressOffset },
                    momiji::v2::convertOperand(instr.operands[0]),
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

        momiji::v2::parser_metadata
        parseDivu(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            if (!matchOp<ops::DataRegister>(instr.operands[1]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[1]),
                    1
                };

                res.result = false;
                res.error  = std::move(error);

                return res;
            }

            if (matchOp<operands::Address>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressIndex,
                      momiji::v2::ParserOperand::AddressOffset },
                    momiji::v2::convertOperand(instr.operands[0]),
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

        momiji::v2::parser_metadata
        parseSwap(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = OneRegisterInstructionParser(instr)(str);

            if (!matchOp<ops::DataRegister>(instr.operands[0]))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister },
                    momiji::v2::convertOperand(instr.operands[0]),
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

        momiji::v2::parser_metadata
        parseExg(std::string_view str, momiji::v2::ParsedInstruction& instr)
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
                    momiji::v2::errors::OperandTypeMismatch error {
                        { momiji::v2::ParserOperand::DataRegister,
                          momiji::v2::ParserOperand::AddressRegister },
                        momiji::v2::convertOperand(instr.operands[i]),
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

        momiji::v2::parser_metadata
        parseOr(std::string_view str, momiji::v2::ParsedInstruction& instr)
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

        momiji::v2::parser_metadata
        parseAnd(std::string_view str, momiji::v2::ParsedInstruction& instr)
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

        momiji::v2::parser_metadata
        parseXor(std::string_view str, momiji::v2::ParsedInstruction& instr)
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
                res.error  = momiji::v2::errors::OperandTypeMismatch {};
                return res;
            }

            for (const auto& op : instr.operands)
            {
                sanitizeRegisters(op, res);
            }

            sanitizeImmediate(op2, res);

            return res;
        }

        momiji::v2::parser_metadata parseNot(std::string_view str,
                                             momiji::v2::ParsedInstruction&)
        {
            return { false, str, "", {} };
        }

        momiji::v2::parser_metadata
        parseCmp(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = CommonInstructionParser(instr)(str);

            const auto& op1 = instr.operands[0];
            const auto& op2 = instr.operands[1];

            if (!matchOp<ops::Immediate>(op1) ||
                (!matchOp<ops::DataRegister>(op2) &&
                 !matchOp<ops::AddressRegister>(op2)))
            {
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::AddressRegister },
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
                momiji::v2::errors::OperandTypeMismatch error {
                    { momiji::v2::ParserOperand::DataRegister,
                      momiji::v2::ParserOperand::Address,
                      momiji::v2::ParserOperand::AddressPre,
                      momiji::v2::ParserOperand::AddressPost,
                      momiji::v2::ParserOperand::AddressOffset,
                      momiji::v2::ParserOperand::AddressIndex },
                    convertOperand(op2),
                    1
                };

                res.result = false;
                res.error  = std::move(error);
                return res;
            }

            switch (convertOperand(op2))
            {
            case momiji::v2::ParserOperand::AddressRegister:
                instr.instructionType = InstructionType::CompareA;
                break;

            default:
                instr.instructionType = InstructionType::Compare;
            }

            switch (convertOperand(op1))
            {
            case momiji::v2::ParserOperand::Immediate:
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

        momiji::v2::parser_metadata
        parseTst(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::Tst;
            return OneRegisterInstructionParser(instr)(str);
        }

        momiji::v2::parser_metadata
        parseJmp(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = SeqNext(
                Whitespace(),
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

        momiji::v2::parser_metadata
        parseJsr(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = parseJmp(str, instr);

            instr.instructionType = InstructionType::JmpSubroutine;

            return res;
        }

        momiji::v2::parser_metadata
        parseBra(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::Branch;

            return res;
        }

        momiji::v2::parser_metadata
        parseBsr(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = parseBra(str, instr);

            instr.instructionType = InstructionType::BranchSubroutine;

            return res;
        }

        momiji::v2::parser_metadata
        parseRts(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::ReturnSubroutine;
            instr.operands[0]     = ops::DataRegister { 0 };
            instr.operands[1]     = ops::DataRegister { 0 };

            return { true, str, "", {} };
        }

        momiji::v2::parser_metadata
        parseBlt(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::LessThan;

            return res;
        }

        momiji::v2::parser_metadata
        parseBle(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::LessEq;

            return res;
        }

        momiji::v2::parser_metadata
        parseBgt(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::GreaterThan;

            return res;
        }

        momiji::v2::parser_metadata
        parseBge(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::GreaterEq;

            return res;
        }

        momiji::v2::parser_metadata
        parseBeq(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::Equal;

            return res;
        }

        momiji::v2::parser_metadata
        parseBne(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            auto res = BranchInstructionParser(instr)(str);

            instr.instructionType = InstructionType::BranchCondition;
            instr.branchCondition = BranchConditions::NotEqual;

            return res;
        }

        momiji::v2::parser_metadata
        parseAsl(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::ArithmeticShiftLeft;

            return ShiftInstructionParser(instr)(str);
        }

        momiji::v2::parser_metadata
        parseAsr(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::ArithmeticShiftRight;

            return ShiftInstructionParser(instr)(str);
        }

        momiji::v2::parser_metadata
        parseLsl(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::LogicalShiftLeft;

            return ShiftInstructionParser(instr)(str);
        }

        momiji::v2::parser_metadata
        parseLsr(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::LogicalShiftRight;

            return ShiftInstructionParser(instr)(str);
        }

        momiji::v2::parser_metadata
        parseDeclare(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            parser_metadata res = (ParseDataType(instr))(str);

            instr.instructionType = InstructionType::Declare;

            auto singleOpParser = [](momiji::v2::ParsedInstruction& instr,
                                     std::uint32_t opNum) {
                return
                    [&instr, opNum](std::string_view str) -> parser_metadata {
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

        momiji::v2::parser_metadata
        parseHcf(std::string_view str, momiji::v2::ParsedInstruction& instr)
        {
            instr.instructionType = InstructionType::HaltCatchFire;

            instr.operands[0] = ops::Immediate {};
            instr.operands[1] = ops::DataRegister { 0 };

            instr.dataType = DataType::Word;

            return { true, str, "", {} };
        }

    } // namespace v2

} // namespace momiji::details
