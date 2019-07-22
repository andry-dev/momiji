#include "bra.h"

#include "../Instructions/Representations.h"
#include "../Instructions/bra.h"

namespace momiji::dec
{
    DecodedInstruction bra(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Bra bits;

        const std::uint16_t val = mem.read16(idx);

        bits.displacement = (val & 0b00000000'11111111);

        ret.data.op1 = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bra;

        return ret;
    }

    DecodedInstruction bsr(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Bsr bits;

        const std::uint16_t val = mem.read16(idx);

        bits.displacement = (val & 0b00000000'11111111);

        ret.data.op1 = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bsr;

        return ret;
    }
} // namespace momiji::dec
