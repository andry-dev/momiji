#include "tst.h"

#include "../Instructions/Representations.h"
#include "../Instructions/tst.h"

#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction tst(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Tst bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size    = (val & 0b00000000'11000000) >> 6;
        bits.regmode = (val & 0b00000000'00111000) >> 3;
        bits.regtype = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.data.operandType[0] = static_cast<OperandType>(bits.regtype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.regmode);

        ret.exec = instr::tst;

        ret.string = "tst" + dataTypeToString(ret.data.size) + " " +
                     opToString(ret.data, 0, mem, idx);

        return ret;
    }
} // namespace momiji::dec
