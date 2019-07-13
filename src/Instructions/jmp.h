#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System jmp(momiji::System& sys, const InstructionData& data);
}
