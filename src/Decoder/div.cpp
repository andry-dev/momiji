#include "div.h"

#include "../Instructions/Representations.h"
#include "../Instructions/div.h"

namespace momiji::dec
{
    DecodedInstruction divs(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::DivS bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Div is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);

        ret.exec = instr::divs;

        return ret;
    }

    DecodedInstruction divu(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::DivU bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Div is always two bytes
        ret.data.size = 2;
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.op2 = OperandType::DataRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        ret.exec = instr::divu;

        return ret;
    }
} // namespace momiji::dec
