#include "sub.h"

#include "../Instructions/Representations.h"
#include "../Instructions/sub.h"
#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction sub(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Sub bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size = (val & 0b00000000'11000000) >> 6;
        bits.othtype = (val & 0b00000000'00111000) >> 3;
        bits.othmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        // sub.* XX, Dn
        if (bits.direction == 0)
        {
            ret.data.op1 = static_cast<OperandType>(bits.othtype);
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.othmode);
            ret.data.op2 = OperandType::DataRegister;
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.datareg);
        }
        // sub.* Dn, XX
        else
        {
            ret.data.op1 = OperandType::DataRegister;
            ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.op2 = static_cast<OperandType>(bits.othtype);
            ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.othmode);
        }

        ret.exec = instr::sub;

        return ret;
    }

    DecodedInstruction suba(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::SubA bits;

        const std::uint16_t val = mem.read16(idx);

        bits.addreg = (val & 0b00001110'00000000) >> 9;
        bits.size = (val & 0b00000001'00000000) >> 8;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        ret.data.size = (bits.size == 0) ? 2 : 4;
        ret.data.op1 = static_cast<OperandType>(bits.srctype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.op2 = OperandType::AddressRegister;
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.addreg);

        ret.exec = instr::suba;

        return ret;
    }

    DecodedInstruction subi(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::SubI bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size = (val & 0b00000000'11000000) >> 6;
        bits.dsttype = (val & 0b00000000'00111000) >> 3;
        bits.dstmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.exec = instr::subi;
        ret.data.op1 = OperandType::Immediate;
        ret.data.mod1 = SpecialAddressingMode::Immediate;
        ret.data.op2 = static_cast<OperandType>(bits.dsttype);
        ret.data.mod2 = static_cast<SpecialAddressingMode>(bits.dstmode);

        return ret;
    }
} // namespace momiji::dec
