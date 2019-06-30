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
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::And bits;

        // and.* *, d*
        if (instr.operands[1].operandType == OperandType::DataRegister)
        {
            bits.datareg = (instr.operands[1].value) & 0b111;
            bits.direction = 0;
        }
        // and.* d*, *
        else
        {
            bits.datareg = (instr.operands[0].value) & 0b111;
            bits.direction = 1;
        }

        bits.othtype = utils::to_val(instr.operands[bits.direction].operandType);
        bits.othmode = getCorrectOpMode(instr, bits.direction);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.direction << 8)
                    | (bits.size << 6)
                    | (bits.othtype << 3)
                    | (bits.othmode);
    }

    void andi(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::AndI bits;

        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;
        bits.size = size & 0b111;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val = instr.operands[0].value;

        bits.dsttype = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dstmode = getCorrectOpMode(instr, 1);

        opcode.val =  (bits.header << 8)
                    | (bits.size << 6)
                    | (bits.dsttype << 3)
                    | (bits.dstmode);
    }
}
