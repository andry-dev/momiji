#pragma once

#include <momiji/Memory.h>
#include <momiji/System.h>
#include <momiji/Types.h>

namespace momiji
{
    struct InstructionData
    {
        std::uint8_t size { 2 };

        OperandType op1;
        SpecialAddressingMode mod1;

        OperandType op2;
        SpecialAddressingMode mod2;

        gsl::span<std::uint16_t> mem;
    };

    using DecodedInstructionFn =
        momiji::System (*)(momiji::System&, const InstructionData& data);

    struct DecodedInstruction
    {
        DecodedInstruction();

        InstructionData data;
        DecodedInstructionFn exec;
    };

    DecodedInstruction decode(ExecutableMemoryView mem, std::uint64_t idx);
} // namespace momiji
