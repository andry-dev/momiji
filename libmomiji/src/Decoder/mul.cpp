#include "mul.h"

#include "../Instructions/Representations.h"
#include "../Instructions/mul.h"

#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction muls(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::MulS bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Mul is always two bytes
        ret.data.size           = 2;
        ret.data.operandType[0] = static_cast<OperandType>(bits.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.srcmode);

        ret.data.operandType[1] = OperandType::DataRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.datareg);

        ret.exec = instr::muls;

        ret.string = "muls " + commonStringConverter(ret.data, mem, idx);

        return ret;
    }

    DecodedInstruction mulu(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::MulU bits;

        const std::uint16_t val = mem.read16(idx);

        bits.datareg = (val & 0b00001110'00000000) >> 9;
        bits.srctype = (val & 0b00000000'00111000) >> 3;
        bits.srcmode = (val & 0b00000000'00000111);

        // Mul is always two bytes
        ret.data.size = 2;

        ret.data.operandType[0] = static_cast<OperandType>(bits.srctype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.srcmode);

        ret.data.operandType[1] = OperandType::DataRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.datareg);
        ret.exec = instr::mulu;

        ret.string = "mulu " + commonStringConverter(ret.data, mem, idx);

        return ret;
    }
} // namespace momiji::dec
