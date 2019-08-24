#include "and.h"

#include "../Instructions/Representations.h"
#include "../Instructions/and.h"
#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction and_instr(ConstExecutableMemoryView mem,
                                 std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::And bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg   = (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size      = (val & 0b00000000'11000000) >> 6;
        bits.othtype   = (val & 0b00000000'00111000) >> 3;
        bits.othmode   = (val & 0b00000000'00000111);

        if (bits.direction == 1)
        {
            ret.data.operandType[0] = OperandType::DataRegister;
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.datareg);

            ret.data.operandType[1] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.othmode);
        }
        else
        {
            ret.data.operandType[0] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.othmode);

            ret.data.operandType[1] = OperandType::DataRegister;
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.datareg);
        }

        momiji::assignNormalSize(ret, bits.size);

        ret.exec = instr::and_instr;

        ret.string = "and" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction andi(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::AndI bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size    = (val & 0b00000000'11000000) >> 6;
        bits.dsttype = (val & 0b00000000'00111000) >> 3;

        ret.data.operandType[0]    = OperandType::Immediate;
        ret.data.addressingMode[0] = SpecialAddressingMode::Immediate;

        ret.data.operandType[1] = OperandType::DataRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.dsttype);

        ret.exec = instr::andi;

        ret.string = "andi" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }
} // namespace momiji::dec
