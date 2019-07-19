#pragma once

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void or_instr(const momiji::Instruction& instr,
                  OpcodeDescription& opcode,
                  std::array<AdditionalData, 2>& additionalData)
    {
        repr::Or bits;

        // or.* *, d*
        if (instr.operands[1].operandType == OperandType::DataRegister)
        {
            bits.datareg   = (instr.operands[1].value) & 0b111;
            bits.direction = 0;
        }
        // or.* d*, *
        else
        {
            bits.datareg   = (instr.operands[0].value) & 0b111;
            bits.direction = 1;
        }

        bits.othtype = instr.operands[bits.direction].value & 0b111;
        bits.othmode = getCorrectOpMode(instr, bits.direction);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.direction << 8) | (bits.size << 6) |
                     (bits.othtype << 3) | (bits.othmode);
    }

    void ori(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::OrI bits;

        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;
        bits.size               = size & 0b111;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val = instr.operands[0].value;

        bits.dsttype = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dstmode = getCorrectOpMode(instr, 1);

        opcode.val = (bits.header << 8) | (bits.size << 6) |
                     (bits.dsttype << 3) | (bits.dstmode);
    }
} // namespace momiji