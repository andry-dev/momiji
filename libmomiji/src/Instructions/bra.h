#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System bra(momiji::System& sys, const InstructionData& data);
    momiji::System bsr(momiji::System& sys, const InstructionData& data);
} // namespace momiji::instr
