#pragma once

#include <momiji/Memory.h>
#include <momiji/System.h>
#include <momiji/Types.h>

namespace momiji
{
    struct InstructionData
    {
        std::uint8_t size { 2 };

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

    DecodedInstruction decode(ExecutableMemoryView mem, std::uint64_t idx);

    namespace utils
    {
        inline int isImmediate(const momiji::InstructionData& instr, int op)
        {
            if ((instr.operandType[op] == OperandType::Immediate) &&
                (instr.addressingMode[op] == SpecialAddressingMode::Immediate))
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

            if (instr.operandType[op] == OperandType::Immediate &&
                instr.addressingMode[op] ==
                    SpecialAddressingMode::AbsoluteShort)
            {
                return 2;
            }

            if ((instr.operandType[op] == OperandType::Immediate) &&
                (instr.addressingMode[op] ==
                 SpecialAddressingMode::AbsoluteLong))
            {
                return 4;
            }

            if ((instr.operandType[op] == OperandType::AddressOffset) ||
                (instr.operandType[op] == OperandType::AddressIndex))
            {
                return 2;
            }

            return 0;
        }

        inline int resolveOp1Size(const momiji::InstructionData& instr, int op)
        {
            if (op == 0)
            {
                return 0;
            }

            return isImmediate(instr, 0);
        }
    } // namespace utils
} // namespace momiji
