#include "cmp.h"

#include "../Instructions/Representations.h"
#include "../Instructions/cmp.h"

#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction cmp(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::Cmp bits;

        const std::uint16_t val = mem[idx];

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.size =    (val & 0b00000000'11000000) >> 6;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.exec = instr::cmp;

        return ret;
    }

    DecodedInstruction cmpa(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::CmpA bits;

        const std::uint16_t val = mem[idx];

        bits.addreg =  (val & 0b00001110'00000000) >> 9;
        bits.size =    (val & 0b00000001'00000000) >> 8;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        switch (bits.size)
        {
        case 0:
            ret.data.size = 2;
            break;

        case 1:
            ret.data.size = 4;
            break;
        }

        ret.data.op2 = OperandType::AddressRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.addreg);
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.exec = instr::cmpa;

        return ret;
    }

    DecodedInstruction cmpi(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::CmpI bits;

        const std::uint16_t val = mem[idx];

        bits.size =    (val & 0b00000000'11000000) >> 6;
        bits.dsttype = (val & 0b00000000'00111000) >> 3;
        bits.dstmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.exec = instr::cmpi;
        ret.data.op1 = OperandType::Immediate;
        ret.data.mod1 = SpecialAddressingMode::Immediate;
        ret.data.op2 = static_cast<OperandType>(bits.dsttype);
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.dstmode);

        return ret;
    }
}
