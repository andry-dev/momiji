#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System muls(momiji::System& sys, const InstructionData& data);
    momiji::System mulu(momiji::System& sys, const InstructionData& data);
} // namespace momiji::instr
