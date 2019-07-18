#pragma once

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void xor_instr(const momiji::Instruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData)
    {
        repr::Xor bits;

        // eor.* d*, *
        bits.datareg = (instr.operands[0].value) & 0b111;

        bits.othtype = utils::to_val(instr.operands[1].operandType);
        bits.othmode = getCorrectOpMode(instr, 1);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.padding << 8) | (bits.size << 6) |
                     (bits.othtype << 3) | (bits.othmode);
    }

    void xori(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::XorI bits;

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
