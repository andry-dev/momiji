#include "add.h"

#include "../Instructions/Representations.h"
#include "../Instructions/add.h"
#include "Utils.h"

namespace momiji
{
    namespace dec
    {
        DecodedInstruction add(gsl::span<std::uint16_t> mem, int idx)
        {
            return {};
        }

        DecodedInstruction adda(gsl::span<std::uint16_t> mem, int idx)
        {
            return  {};
        }

        DecodedInstruction addi(gsl::span<std::uint16_t> mem, int idx)
        {
            DecodedInstruction ret;

            repr::AddI repr;

            std::uint16_t val = mem[idx];

            repr.size =     (val & 0b00000000'11000000) >> 6;
            repr.dsttype =  (val & 0b0000000'00111000) >> 3;
            repr.dstmode =      (val & 0b0000000'00000111);

            momiji::assignNormalSize(ret, repr.size);

            ret.exec = instr::addi;
            ret.data.op2 = static_cast<OperandType>(repr.dsttype);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(repr.dstmode);

            return ret;
        }
    }
}
