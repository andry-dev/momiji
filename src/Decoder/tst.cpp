#include "tst.h"

#include "../Instructions/Representations.h"
#include "../Instructions/tst.h"

#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction tst(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Tst bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size =    (val & 0b00000000'11000000) >> 6;
        bits.regmode = (val & 0b00000000'00111000) >> 3;
        bits.regtype = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.data.op1 = static_cast<OperandType>(bits.regtype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.regmode);

        ret.exec = instr::tst;

        return ret;
    }
}
