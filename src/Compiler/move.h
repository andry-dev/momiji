#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include <Parser.h>
#include <Utils.h>

#include "../Instructions/Representations.h"

namespace momiji
{
    void move(const momiji::Instruction& instr,
              MemoryType& memory,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::Move bits;
        bits.header = 0b00;

        // This Move requires special handling of sizes.
        std::int8_t size = move_sizeconv[utils::to_val(instr.dataType)];

        // Byte size
        bits.size = size & 0b011;

        // Destination
        bits.dsttype = utils::to_val(instr.operands[1].operandType);;
        bits.dstmode = getCorrectOpMode(instr, 1);

        // Source
        std::uint32_t srcval = utils::to_val(instr.operands[0].operandType);
        std::uint8_t srcmode = utils::to_val(instr.operands[0].specialAddressingMode);

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                additionalData.val = instr.operands[0].value;
                additionalData.cnt = move_tobyte[size];
                break;

            default:
                break;
            }
        }

        opcode.val =   (bits.header << 14)
                     | (bits.size << 12)
                     | (bits.dstmode << 9)
                     | (bits.dsttype << 6)
                     | (bits.srctype << 3)
                     | (bits.srcmode);
    }
}
