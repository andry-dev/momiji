#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System divs(momiji::System sys, const InstructionData& data);
    momiji::System divu(momiji::System sys, const InstructionData& data);
}
