#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System divs(momiji::System& sys, const InstructionData& data);
    momiji::System divu(momiji::System& sys, const InstructionData& data);
} // namespace momiji::instr
