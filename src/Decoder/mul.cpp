#include "mul.h"

#include "../Instructions/Representations.h"
#include "../Instructions/mul.h"

namespace momiji::dec
{
    DecodedInstruction muls(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::MulS bits;

        const std::uint16_t val = mem[idx];
        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Mul is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);

        ret.exec = instr::muls;

        return ret;
    }

    DecodedInstruction mulu(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::MulU bits;

        const std::uint16_t val = mem[idx];
        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Mul is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        ret.exec = instr::mulu;

        return ret;
    }
}
