#include "jmp.h"

#include "../Instructions/Representations.h"
#include "../Instructions/jmp.h"

#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction jmp(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Jmp bits;

        const std::uint16_t val = mem.read16(idx);

        bits.regtype = (val & 0b00000000'00111000) >> 3;
        bits.regmode = (val & 0b00000000'00000111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.regtype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.regmode);

        ret.exec = instr::jmp;

        ret.string = "jmp " + opToString(ret.data, 0, mem, idx);

        return ret;
    }

    DecodedInstruction jsr(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Jsr bits;

        const std::uint16_t val = mem.read16(idx);

        bits.regtype = (val & 0b00000000'00111000) >> 3;
        bits.regmode = (val & 0b00000000'00000111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.regtype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.regmode);

        ret.exec = instr::jsr;

        ret.string = "jsr " + opToString(ret.data, 0, mem, idx);

        return ret;
    }
} // namespace momiji::dec
