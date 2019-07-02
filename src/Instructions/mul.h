#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System muls(momiji::System& sys, const InstructionData& data);
    momiji::System mulu(momiji::System& sys, const InstructionData& data);
}
