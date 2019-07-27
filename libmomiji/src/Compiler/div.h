#pragma once

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void divs(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::DivS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        handleAdditionalData(instr, additionalData);

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.padding << 6) | (bits.srctype << 3) | (bits.srcmode);
    }

    void divu(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::DivS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        handleAdditionalData(instr, additionalData);

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.padding << 6) | (bits.srctype << 3) | (bits.srcmode);
    }
} // namespace momiji
