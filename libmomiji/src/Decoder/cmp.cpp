#include "cmp.h"

#include "../Instructions/Representations.h"
#include "../Instructions/cmp.h"

#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction cmp(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Cmp bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.size    = (val & 0b00000000'11000000) >> 6;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.data.operandType[1] = OperandType::DataRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.datareg);
        ret.data.operandType[0] = static_cast<OperandType>(bits.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.srcmode);

        ret.exec = instr::cmp;

        ret.string = "cmp" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction cmpa(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::CmpA bits;

        const std::uint16_t val = mem.read16(idx);

        bits.addreg  = (val & 0b00001110'00000000) >> 9;
        bits.size    = (val & 0b00000001'00000000) >> 8;
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

        ret.data.operandType[0] = static_cast<OperandType>(bits.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.operandType[1] = OperandType::AddressRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.addreg);

        ret.exec = instr::cmpa;

        ret.string = "cmpa" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction cmpi(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::CmpI bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size    = (val & 0b00000000'11000000) >> 6;
        bits.dsttype = (val & 0b00000000'00111000) >> 3;
        bits.dstmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.exec                   = instr::cmpi;
        ret.data.operandType[0]    = OperandType::Immediate;
        ret.data.addressingMode[0] = SpecialAddressingMode::Immediate;
        ret.data.operandType[1]    = static_cast<OperandType>(bits.dsttype);
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.dstmode);

        ret.string = "cmpi" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }
} // namespace momiji::dec
