#pragma once

#include <momiji/Decoder.h>
#include <momiji/System.h>

namespace momiji::instr
{
    momiji::System swap(momiji::System& sys, const InstructionData& instr);
}
