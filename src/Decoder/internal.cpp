#include "internal.h"

#include "../Instructions/internal.h"

namespace momiji::dec
{
    DecodedInstruction momijiInternal(gsl::span<std::uint16_t> mem, int idx)
    {
        DecodedInstruction ret;

        std::int16_t controlcode = mem[idx + 1];

        switch (controlcode)
        {
        case 1:
            ret.exec = instr::handleBreakpoint;
            break;
        }

        ret.data.size = 2;
        ret.data.op1 = OperandType::Immediate;

        return ret;
    }
}
