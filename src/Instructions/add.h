#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji
{
    namespace instr
    {
        momiji::System add(momiji::System sys, const InstructionData& instr);
    }
}
