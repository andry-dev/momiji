#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System cmp(momiji::System& sys, const InstructionData& instr);
    momiji::System cmpa(momiji::System& sys, const InstructionData& instr);
    momiji::System cmpi(momiji::System& sys, const InstructionData& instr);
}
