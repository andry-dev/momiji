#pragma once

#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <momiji/Parser.h>
#include <momiji/Utils.h>

namespace momiji
{
    void rts(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Rts bits;
        opcode.val = bits.header;
    }
} // namespace momiji
