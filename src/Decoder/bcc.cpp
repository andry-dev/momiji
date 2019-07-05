#include "bra.h"

#include "../Instructions/Representations.h"
#include "../Instructions/bcc.h"

namespace momiji::dec
{
    DecodedInstruction bcc(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Bcc bits;

        const std::uint16_t val = mem.read16(idx);

        bits.condition =    (val & 0b00001111'00000000) >> 8;
        bits.displacement = (val & 0b00000000'11111111);

        ret.data.op1 = static_cast<OperandType>(bits.condition);
        ret.data.op2 = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bcc;

        return ret;
    }
}
