#pragma once

#include "./Utils.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void jmp(const momiji::Instruction& instr,
             MemoryType&,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Jmp bits;

        const auto& op = instr.operands[0];

        bits.regtype = utils::to_val(op.operandType);
        bits.regmode = getCorrectOpMode(instr, 0);

        if (op.operandType == OperandType::Immediate &&
            (op.specialAddressingMode == SpecialAddressingMode::AbsoluteLong ||
             op.specialAddressingMode == SpecialAddressingMode::AbsoluteShort))
        {
            additionalData[0].val = op.value;
            additionalData[0].cnt = 4;
        }

        opcode.val =   (bits.header << 6)
                     | (bits.regtype << 3)
                     | (bits.regmode);
    }
}
