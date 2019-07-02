#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System move(momiji::System& sys, const InstructionData& data);
}
