#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System and_instr(momiji::System& sys, const InstructionData& mem);
    momiji::System andi(momiji::System& sys, const InstructionData& mem);
} // namespace momiji::instr
