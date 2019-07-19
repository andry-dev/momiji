#include "jmp.h"

#include "../Instructions/Representations.h"
#include "../Instructions/jmp.h"

namespace momiji::dec
{
    DecodedInstruction jmp(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Jmp bits;

        const std::uint16_t val = mem.read16(idx);

        bits.regtype = (val & 0b00000000'00111000) >> 3;
        bits.regmode = (val & 0b00000000'00000111);

        ret.data.op1  = static_cast<OperandType>(bits.regtype);
        ret.data.mod1 = static_cast<SpecialAddressingMode>(bits.regmode);

        ret.exec = instr::jmp;

        return ret;
    }
} // namespace momiji::dec
