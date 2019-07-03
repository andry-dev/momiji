#pragma once

#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void breakpoint(const momiji::Instruction& instr,
                    MemoryType&,
                    OpcodeDescription& opcode,
                    std::array<AdditionalData, 2>& additionalData)
    {
        additionalData[0].cnt = 2;
        additionalData[0].val = 1;

        opcode.val = 0xFFFF;
    }
}
