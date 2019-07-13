#pragma once

#include <Decoder.h>
#include <System.h>
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
    } // namespace instr
} // namespace momiji
