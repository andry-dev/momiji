#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System or_instr(momiji::System& sys, const InstructionData& data);
    momiji::System ori(momiji::System& sys, const InstructionData& data);
} // namespace momiji::instr
