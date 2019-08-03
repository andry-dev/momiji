#pragma once

#include <Decoder.h>
#include <System.h>
#include <iostream>

namespace momiji
{
    namespace instr
    {
        inline momiji::System illegal(momiji::System& sys,
                                      const InstructionData& /*data*/)
        {
            std::cout << "ILLEGAL INSTRUCTION\n";
            return sys;
        }
    } // namespace instr
} // namespace momiji
