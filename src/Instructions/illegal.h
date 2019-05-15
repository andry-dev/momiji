#pragma once

#include <System.h>
#include <Decoder.h>
#include <iostream>

namespace momiji
{
    namespace instr
    {
        inline momiji::System illegal(momiji::System sys, const InstructionData& data)
        {
            std::cout << "ILLEGAL INSTRUCTION\n";
            return sys;
        }
    }
}
