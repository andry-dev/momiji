#include "sub.h"

#include "../Instructions/Representations.h"
#include "../Instructions/sub.h"
#include "Utils.h"

namespace momiji
{
    namespace dec
    {
        DecodedInstruction sub(gsl::span<std::uint16_t> mem, int idx)
        {
            return {};
        }

        DecodedInstruction suba(gsl::span<std::uint16_t> mem, int idx)
        {
            return  {};
        }

        DecodedInstruction subi(gsl::span<std::uint16_t> mem, int idx)
        {
            DecodedInstruction ret;

            repr::SubI repr;

            std::uint16_t val = mem[idx];

            repr.size =    (val & 0b00000000'11000000) >> 6;
            repr.dstmode = (val & 0b00000000'00111000) >> 3;
            repr.dst =     (val & 0b00000000'00000111);

            momiji::assignNormalSize(ret, repr.size);

            ret.exec = instr::subi;
            ret.data.op2 = static_cast<OperandType>(repr.dstmode);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(repr.dst);

            return ret;
        }
    }
}
