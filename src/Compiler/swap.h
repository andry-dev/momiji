#pragma once

#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void swap(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::Swap bits;

        bits.datareg = instr.operands[0].value & 0b111;

        opcode.val = (bits.header << 3) | (bits.datareg);
    }
} // namespace momiji
