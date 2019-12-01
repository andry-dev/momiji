#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::instr
{
    momiji::System sub(momiji::System& sys, const InstructionData& data);
    momiji::System subi(momiji::System& sys, const InstructionData& data);
    momiji::System suba(momiji::System& sys, const InstructionData& data);
} // namespace momiji::instr
