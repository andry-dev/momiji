#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji
{
    namespace instr
    {
        momiji::System move(momiji::System sys, const InstructionData& data);
    }
}
