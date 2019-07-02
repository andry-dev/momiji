#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System add(momiji::System& sys, const InstructionData& instr);
    momiji::System addi(momiji::System& sys, const InstructionData& instr);
    momiji::System adda(momiji::System& sys, const InstructionData& instr);
}
