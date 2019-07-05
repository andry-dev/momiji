#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System handleBreakpoint(momiji::System& sys, const InstructionData& instr);
    momiji::System hcf(momiji::System& sys, const InstructionData& instr);
}
