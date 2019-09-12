#include "bra.h"

#include "../Instructions/Representations.h"
#include "../Instructions/bra.h"

namespace momiji::dec
{
    DecodedInstruction bra(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Bra bits;

        const std::uint16_t val = *mem.read16(idx);

        bits.displacement = (val & 0b00000000'11111111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bra;

        std::int16_t displ = bits.displacement;

        if (displ == 0)
        {
            displ = std::int16_t(*mem.read16(idx + 2));
        }

        ret.string = "bra " + std::to_string(displ);

        return ret;
    }

    DecodedInstruction bsr(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        repr::Bsr bits;

        const std::uint16_t val = *mem.read16(idx);

        bits.displacement = (val & 0b00000000'11111111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bsr;

        std::int16_t displ = bits.displacement;

        if (displ == 0)
        {
            displ = std::int16_t(*mem.read16(idx + 2));
        }

        ret.string = "bsr " + std::to_string(displ);

        return ret;
    }
} // namespace momiji::dec
