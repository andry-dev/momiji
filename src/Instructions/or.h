#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System or_instr(momiji::System sys, const InstructionData& instr);
    momiji::System ori(momiji::System sys, const InstructionData& instr);
}
