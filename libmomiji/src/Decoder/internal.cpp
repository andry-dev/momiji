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
            ret.exec   = instr::hcf;
            ret.string = "hcf";
            break;
        case 1:
            ret.exec   = instr::handleBreakpoint;
            ret.string = "breakpoint";
            break;
        }

        ret.data.size = 2;

        ret.data.operandType[0] = OperandType::Immediate;

        return ret;
    }
} // namespace momiji::dec
