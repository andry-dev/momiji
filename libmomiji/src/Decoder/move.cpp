#include "move.h"

#include "../Instructions/illegal.h"
#include "../Instructions/move.h"

#include "../Instructions/Representations.h"

#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction move(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        std::uint16_t val = mem.read16(idx);

        repr::Move repr;

        repr.size    = (val & 0b00110000'00000000) >> 12;
        repr.dstmode = (val & 0b00001110'00000000) >> 9;
        repr.dsttype = (val & 0b00000001'11000000) >> 6;
        repr.srctype = (val & 0b00000000'00111000) >> 3;
        repr.srcmode = (val & 0b00000000'00000111);

        switch (repr.size)
        {
        case 0b01:
            ret.data.size = 1;
            break;

        case 0b11:
            ret.data.size = 2;
            break;

        case 0b10:
            ret.data.size = 4;
            break;

        default:
            ret.exec = momiji::instr::illegal;
            return ret;
        }

        ret.data.operandType[0] = static_cast<OperandType>(repr.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(repr.srcmode);
        ret.data.operandType[1] = static_cast<OperandType>(repr.dsttype);
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(repr.dstmode);
        ret.exec = momiji::instr::move;

        ret.string = "move" + dataTypeToString(ret.data.size);

        return ret;
    }

} // namespace momiji::dec
