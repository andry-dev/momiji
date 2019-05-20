#include "exg.h"

#include "../Instructions/Representations.h"
#include "../Instructions/exg.h"

namespace momiji::dec
{
    DecodedInstruction exg(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        repr::Exg bits;

        const std::uint16_t val = mem[idx];

        bits.mode = (mem[idx] & 0b00000000'11111000) >> 3;
        bits.datareg = (mem[idx] & 0b00001110'00000000) >> 9;
        bits.addreg = (mem[idx] & 0b00000000'00000111);

        switch (bits.mode)
        {
        case 0b01000:
            ret.data.op1 = OperandType::DataRegister;
            ret.data.op2 = OperandType::DataRegister;
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.addreg);
            break;

        case 0b01001:
            ret.data.op1 = OperandType::AddressRegister;
            ret.data.op2 = OperandType::AddressRegister;
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.addreg);
            break;

        case 0b10001:
            ret.data.op1 = OperandType::DataRegister;
            ret.data.op2 = OperandType::AddressRegister;
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.addreg);
            break;
        }

        ret.exec = instr::exg;

        return ret;
    }
}
