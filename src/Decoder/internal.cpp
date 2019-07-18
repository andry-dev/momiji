#include "internal.h"

#include "../Instructions/internal.h"

namespace momiji::dec
{
    DecodedInstruction momijiInternal(ExecutableMemoryView mem,
                                      std::uint64_t idx)
    {
        DecodedInstruction ret;

        const std::uint16_t controlcode = mem.read16(idx + 2);

        switch (controlcode)
        {
        case 0:
            ret.exec = instr::hcf;
            break;
        case 1:
            ret.exec = instr::handleBreakpoint;
            break;
        }

        ret.data.size = 2;
        ret.data.op1  = OperandType::Immediate;

        return ret;
    }
} // namespace momiji::dec
