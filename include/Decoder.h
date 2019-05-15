#pragma once

#include <System.h>
#include <Types.h>

#include <gsl/span>

namespace momiji
{
    struct InstructionData
    {
        std::uint8_t size{2};

        OperandType op1 : 3;
        SpecialAddressingMode mod1 : 3;

        OperandType op2 : 3;
        SpecialAddressingMode mod2 : 3;

        gsl::span<std::uint16_t> mem;
    };

    using DecodedInstructionFn = momiji::System(*)(momiji::System,
                                                   const InstructionData& data);

    struct DecodedInstruction
    {
        DecodedInstruction();

        InstructionData data;
        DecodedInstructionFn exec;
    };

    DecodedInstruction decode(gsl::span<std::uint16_t> mem, int idx);
}
