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
        std::uint32_t dstval = utils::to_val(instr.operands[1].operandType);;
        std::uint8_t dstmode = utils::to_val(instr.operands[1].specialAddressingMode);;

        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister: [[fallthrough]];
        case OperandType::DataRegister:
            dstmode = utils::to_val(instr.operands[1].operandType);
            break;

        default:
            break;
        }

        bits.dst = dstval & 0b111;
        bits.dstmode = dstmode & 0b111;

        // Source
        std::uint32_t srcval = utils::to_val(instr.operands[0].operandType);
        std::uint8_t srcmode = utils::to_val(instr.operands[0].specialAddressingMode);

        switch (instr.operands[0].operandType)
        {
        case OperandType::DataRegister: [[fallthrough]];
        case OperandType::AddressRegister:
            srcval = instr.operands[0].value & 0b0111;
            srcmode = utils::to_val(instr.operands[0].operandType);
            break;

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

        default:
            break;
        }

        bits.srcmode = srcmode & 0b111;
        bits.src = srcval & 0b111;

        opcode.val =   (bits.header << 14)
                     | (bits.size << 12)
                     | (bits.dst << 9)
                     | (bits.dstmode << 6)
                     | (bits.srcmode << 3)
                     | (bits.src);
    }
}
