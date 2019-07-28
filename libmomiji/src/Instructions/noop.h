#pragma once

#include <Decoder.h>
#include <System.h>
#include <iostream>

namespace momiji::instr
{
    momiji::System noop(momiji::System& sys, const InstructionData& /*data*/);
} // namespace momiji::instr
