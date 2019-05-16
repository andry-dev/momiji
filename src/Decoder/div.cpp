#include "div.h"

#include "../Instructions/Representations.h"
#include "../Instructions/div.h"

namespace momiji::dec
{
    DecodedInstruction divs(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::DivS bits;

        const std::uint16_t val = mem[idx];
        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srcmode = (val & 0b00000000'00111000) >> 3;
        bits.src =     (val & 0b00000000'00000111);

        // Div is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srcmode);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.src);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);

        ret.exec = instr::divs;

        return ret;
    }

    DecodedInstruction divu(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::DivS bits;

        const std::uint16_t val = mem[idx];
        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srcmode = (val & 0b00000000'00111000) >> 3;
        bits.src =     (val & 0b00000000'00000111);

        // Div is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srcmode);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.src);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        ret.exec = instr::divu;

        return ret;
    }
}
