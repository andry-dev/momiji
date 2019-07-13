#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System or_instr(momiji::System& sys, const InstructionData& instr);
    momiji::System ori(momiji::System& sys, const InstructionData& instr);
} // namespace momiji::instr
