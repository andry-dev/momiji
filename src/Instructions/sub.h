#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System sub(momiji::System sys, const InstructionData& mem);
    momiji::System subi(momiji::System sys, const InstructionData& mem);
    momiji::System suba(momiji::System sys, const InstructionData& mem);
}
