#include "add.h"

#include "../Instructions/Representations.h"
#include "../Instructions/add.h"
#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction add(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Add bits;

        const std::uint16_t val = *mem.read16(idx);

        bits.datareg   = (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size      = (val & 0b00000000'11000000) >> 6;
        bits.othtype   = (val & 0b00000000'00111000) >> 3;
        bits.othmode   = (val & 0b00000000'00000111);

        assignNormalSize(ret, bits.size);

        if (bits.direction == 0)
        {
            ret.data.operandType[0] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.othmode);
            ret.data.operandType[1] = OperandType::DataRegister;
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.datareg);
        }
        else
        {
            ret.data.operandType[0] = OperandType::DataRegister;
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.operandType[1] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.othmode);
        }

        ret.exec = instr::add;

        ret.string = "add" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction adda(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::AddA bits;

        const std::uint16_t val = *mem.read16(idx);

        bits.addreg  = (val & 0b00001110'00000000) >> 9;
        bits.size    = (val & 0b00000001'00000000) >> 8;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        ret.data.size = (bits.size == 0) ? 2 : 4;

        ret.data.operandType[0] = static_cast<OperandType>(bits.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.srcmode);
        ret.data.operandType[1] = OperandType::AddressRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.addreg);

        ret.exec = instr::adda;

        ret.string = "adda" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction addi(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::AddI repr;

        std::uint16_t val = *mem.read16(idx);

        repr.size    = (val & 0b00000000'11000000) >> 6;
        repr.dsttype = (val & 0b0000000'00111000) >> 3;
        repr.dstmode = (val & 0b0000000'00000111);

        momiji::assignNormalSize(ret, repr.size);

        ret.exec                   = instr::addi;
        ret.data.operandType[0]    = OperandType::Immediate;
        ret.data.addressingMode[0] = SpecialAddressingMode::Immediate;
        ret.data.operandType[1]    = static_cast<OperandType>(repr.dsttype);
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(repr.dstmode);

        ret.string = "addi" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }
} // namespace momiji::dec
