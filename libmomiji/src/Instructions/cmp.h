#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System cmp(momiji::System& sys, const InstructionData& instr);
    momiji::System cmpa(momiji::System& sys, const InstructionData& instr);
    momiji::System cmpi(momiji::System& sys, const InstructionData& instr);
} // namespace momiji::instr
