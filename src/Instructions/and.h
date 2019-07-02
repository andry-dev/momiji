#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System and_instr(momiji::System& sys, const InstructionData& mem);
    momiji::System andi(momiji::System& sys, const InstructionData& mem);
}
