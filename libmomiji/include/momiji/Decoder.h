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
} // namespace momiji
