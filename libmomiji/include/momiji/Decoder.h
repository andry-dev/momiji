#pragma once

#include <momiji/Memory.h>
#include <momiji/System.h>
#include <momiji/Types.h>

namespace momiji
{
    struct InstructionData
    {
        std::int8_t size { 2 };

        std::array<OperandType, 2> operandType;
        std::array<SpecialAddressingMode, 2> addressingMode;
    };

    using InstructionString = std::string;

    using DecodedInstructionFn =
        momiji::System (*)(momiji::System&, const InstructionData& data);

    struct DecodedInstruction
    {
        DecodedInstruction();

        InstructionData data;
        InstructionString string;
        DecodedInstructionFn exec;
    };

    DecodedInstruction decode(momiji::ExecutableMemoryView mem,
                              std::int64_t idx);

    namespace utils
    {
        inline std::int8_t isImmediate(const momiji::InstructionData& instr,
                                       std::int8_t opNum)
        {
            const auto& op   = asl::saccess(instr.operandType, opNum);
            const auto& mode = asl::saccess(instr.addressingMode, opNum);

            if ((op == OperandType::Immediate) &&
                (mode == SpecialAddressingMode::Immediate))
            {
                if ((instr.size == 1) || (instr.size == 2))
                {
                    return 2;
                }
                else
                {
                    return 4;
                }
            }

            if ((op == OperandType::Immediate) &&
                (mode == SpecialAddressingMode::AbsoluteShort))
            {
                return 2;
            }

            if ((op == OperandType::Immediate) &&
                (mode == SpecialAddressingMode::AbsoluteLong))
            {
                return 4;
            }

            if ((op == OperandType::AddressOffset) ||
                (op == OperandType::AddressIndex))
            {
                return 2;
            }

            return 0;
        }

        inline std::int8_t resolveOp1Size(const momiji::InstructionData& instr,
                                          std::int8_t op)
        {
            if (op == 0)
            {
                return 0;
            }

            return isImmediate(instr, 0);
        }
    } // namespace utils
} // namespace momiji
