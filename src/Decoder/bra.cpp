#include "bra.h"

#include "../Instructions/Representations.h"
#include "../Instructions/bra.h"

namespace momiji::dec
{
    DecodedInstruction bra(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::Bra bits;

        const std::uint16_t val = mem[idx];

        bits.displacement = (val & 0b00000000'11111111);

        ret.data.op1 = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bra;

        return ret;
    }
}
