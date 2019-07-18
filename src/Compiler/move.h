#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"
#include <Memory.h>
#include <Parser.h>
#include <Utils.h>

#include "../Instructions/Representations.h"

namespace momiji
{
    void move(const momiji::Instruction& instr, OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::Move bits;
        bits.header = 0b00;

        // This Move requires special handling of sizes.
        const std::int8_t size = move_sizeconv[utils::to_val(instr.dataType)];

        // Byte size
        bits.size = size & 0b011;

        // Destination
        bits.dsttype = utils::to_val(instr.operands[1].operandType);
        bits.dstmode = getCorrectOpMode(instr, 1);

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        handleAdditionalData(instr, additionalData);

        opcode.val = (bits.header << 14) | (bits.size << 12) |
                     (bits.dstmode << 9) | (bits.dsttype << 6) |
                     (bits.srctype << 3) | (bits.srcmode);
    }
} // namespace momiji
