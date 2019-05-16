#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void divs(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::DivS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.src = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = utils::to_val(instr.operands[0].specialAddressingMode);

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
        }

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.padding << 6)
                    | (bits.srcmode << 3)
                    | (bits.src);
    }

    void divu(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::DivS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.src = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = utils::to_val(instr.operands[0].specialAddressingMode);

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
        }

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.padding << 6)
                    | (bits.srcmode << 3)
                    | (bits.src);
    }
}
