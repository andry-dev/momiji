#include "internal.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System handleBreakpoint(momiji::System& sys, const InstructionData& instr)
    {
        std::cout << "we\n";
        return sys;
    }
}
