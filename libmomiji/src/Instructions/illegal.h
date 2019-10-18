#pragma once

#include <Decoder.h>
#include <System.h>
#include <iostream>

namespace momiji::instr
{
    inline momiji::System illegal(momiji::System& sys,
                                  const InstructionData& /*data*/)
    {
        sys.trap = traps::IllegalInstruction {};
        return sys;
    }
} // namespace momiji::instr
