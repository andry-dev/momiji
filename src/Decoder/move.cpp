#include "move.h"

#include "../Instructions/move.h"
#include "../Instructions/illegal.h"

#include "../Instructions/Representations.h"

namespace momiji
{
    namespace dec
    {
        DecodedInstruction move(gsl::span<std::uint16_t> mem, int idx)
        {
            DecodedInstruction ret;

            std::uint16_t val = mem[idx];

            repr::Move repr;
            repr.size =     (val & 0b00110000'00000000) >> 12;
            repr.dst =      (val & 0b00001110'00000000) >> 9;
            repr.dstmode =  (val & 0b00000001'11000000) >> 6;
            repr.src =      (val & 0b00000000'00111000) >> 3;
            repr.srcmode =  (val & 0b00000000'00000111);

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

            ret.data.mem = mem.subspan(idx);
            ret.data.op1 = static_cast<OperandType>(repr.srcmode);
            ret.data.mod1 = static_cast<SpecialAddressingMode>(repr.src);
            ret.data.op2 = static_cast<OperandType>(repr.dstmode);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(repr.dst);
            ret.exec = momiji::instr::move;

            return ret;
        }

    }
}
