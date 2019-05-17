#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void muls(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::MulS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                additionalData.val = instr.operands[0].value;
                additionalData.cnt = 2;
                break;
            }
            break;

        default:
            break;
        }

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.padding << 6)
                    | (bits.srctype << 3)
                    | (bits.srcmode);
    }

    void mulu(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::MulU bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                additionalData.val = instr.operands[0].value;
                additionalData.cnt = 2;
                break;
            }
            break;

        default:
            break;
        }

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.padding << 6)
                    | (bits.srctype << 3)
                    | (bits.srcmode);
    }
}
