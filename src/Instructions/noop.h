#pragma once

#include <System.h>
#include <Decoder.h>
#include <iostream>

namespace momiji
{
    namespace instr
    {
        momiji::System noop(momiji::System& sys, const InstructionData& data)
        {
            std::cout << "NOOP\n";
            return sys;
        }
    }
}
