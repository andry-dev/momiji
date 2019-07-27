#include "or.h"

#include "../Instructions/Representations.h"
#include "../Instructions/or.h"

#include "Utils.h"

namespace momiji::dec
{
    DecodedInstruction or_instr(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Or bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg   = (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size      = (val & 0b00000000'11000000) >> 6;
        bits.othtype   = (val & 0b00000000'00111000) >> 3;
        bits.othmode   = (val & 0b00000000'00000111);

        assignNormalSize(ret, bits.size);

        // or d*, *
        if (bits.direction == 1)
        {
            ret.data.operandType[0] = OperandType::DataRegister;
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.datareg);
            ret.data.operandType[1] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.othmode);
        }
        // or *, d*
        else
        {
            ret.data.operandType[0] = static_cast<OperandType>(bits.othtype);
            ret.data.addressingMode[0] =
                static_cast<SpecialAddressingMode>(bits.othmode);
            ret.data.operandType[1] = OperandType::DataRegister;
            ret.data.addressingMode[1] =
                static_cast<SpecialAddressingMode>(bits.datareg);
        }

        ret.exec = instr::or_instr;

        ret.string = "or" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction ori(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::OrI bits;

        const std::uint16_t val = mem.read16(idx);

        bits.size    = (val & 0b00000000'11000000) >> 6;
        bits.dsttype = (val & 0b00000000'00111000) >> 3;
        bits.dstmode = (val & 0b00000000'00000111);

        momiji::assignNormalSize(ret, bits.size);

        ret.exec = instr::ori;

        ret.data.operandType[0]    = OperandType::Immediate;
        ret.data.addressingMode[0] = SpecialAddressingMode::Immediate;
        ret.data.operandType[1]    = static_cast<OperandType>(bits.dsttype);
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.dstmode);

        ret.string = "ori" + dataTypeToString(ret.data.size) + " " +
                     commonStringConverter(ret.data, mem, idx);

        return ret;
    }
} // namespace momiji::dec
