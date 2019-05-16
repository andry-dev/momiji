#include "or.h"

#include "../Instructions/Representations.h"
#include "../Instructions/or.h"

#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction or_instr(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::Or bits;

        const std::uint16_t val = mem[idx];

        bits.datareg =   (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size =      (val & 0b00000000'11000000) >> 6;
        bits.othmode =   (val & 0b00000000'00111000) >> 3;
        bits.oth =       (val & 0b00000000'00000111);

        assignNormalSize(ret, bits.size);

        // or d*, *
        if (bits.direction == 1)
        {
            ret.data.op1 = OperandType::DataRegister;
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.op2 = static_cast<OperandType>(bits.othmode);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.oth);
        }
        // or *, d*
        else
        {
            ret.data.op1 = static_cast<OperandType>(bits.othmode);
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.oth);
            ret.data.op2 = OperandType::DataRegister;
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        }

        ret.exec = instr::or_instr;

        return ret;
    }

    DecodedInstruction ori(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::OrI bits;

        const std::uint16_t val = mem[idx];

        bits.size =    (val & 0b00000000'11000000) >> 6;
        bits.dstmode = (val & 0b00000000'00111000) >> 3;
        bits.dst =     (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.exec = instr::ori;
        ret.data.op2 = static_cast<OperandType>(bits.dstmode);
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.dst);

        return ret;
    }
}
