#pragma once

#include <momiji/Decoder.h>
#include <momiji/System.h>

namespace momiji::instr
{
    momiji::System rts(momiji::System& sys,
                       const momiji::InstructionData& instr);
}
