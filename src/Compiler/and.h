#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void and_instr(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::Or bits;

        // or.* *, d*
        if (instr.operands[1].operandType == OperandType::DataRegister)
        {
            bits.datareg = (instr.operands[1].value) & 0b111;
            bits.direction = 0;
            bits.othmode = utils::to_val(instr.operands[0].operandType) & 0b111;
            bits.oth = instr.operands[0].value & 0b111;
        }
        // or.* d*, *
        else
        {
            bits.datareg = (instr.operands[0].value) & 0b111;
            bits.direction = 1;
            bits.othmode = utils::to_val(instr.operands[1].operandType) & 0b111;
            bits.oth = instr.operands[1].value & 0b111;
        }

        bits.size = utils::to_val(instr.dataType) & 0b11;

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.direction << 8)
                    | (bits.size << 6)
                    | (bits.othmode << 3)
                    | (bits.oth);
    }

    void andi(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::OrI bits;

        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;
        bits.size = size & 0b111;

        additionalData.cnt = tobyte[size];
        additionalData.val = instr.operands[0].value;

        bits.dstmode = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dst = instr.operands[1].value;

        opcode.val =  (bits.header << 8)
                    | (bits.size << 6)
                    | (bits.dstmode << 3)
                    | (bits.dst);
    }
}
