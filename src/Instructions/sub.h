#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji
{
    namespace instr
    {
        momiji::System sub(momiji::System sys, const InstructionData& mem);

        momiji::System subi(momiji::System sys, const InstructionData& mem);
    }
}
