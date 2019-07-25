#include "internal.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System handleBreakpoint(momiji::System& sys,
                                    const InstructionData& instr)
    {
        std::cout << "we\n";
        return sys;
    }

    momiji::System hcf(momiji::System& sys, const InstructionData& instr)
    {
        sys.cpu.programCounter.address = 0xFFFFFFFF;

        return sys;
    }
} // namespace momiji::instr
