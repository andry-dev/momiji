#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System handleBreakpoint(momiji::System& sys,
                                    const InstructionData& instr);
    momiji::System hcf(momiji::System& sys, const InstructionData& instr);
} // namespace momiji::instr
