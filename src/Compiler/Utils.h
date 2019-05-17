#pragma once

#include <Parser.h>
#include <Utils.h>
#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

namespace momiji
{
    union OpcodeDescription
    {
        std::uint16_t val;
        struct
        {
            std::uint8_t lower;
            std::uint8_t higher;
        };
    };

    struct AdditionalData
    {
        union
        {
            std::uint32_t val;
            std::uint16_t arr16[2];
            std::uint8_t arr8[4];
        };
        std::uint8_t cnt = 0;
    };

    std::uint8_t getCorrectOpMode(const momiji::Instruction& instr, int num)
    {
        switch (instr.operands[num].operandType)
        {
        case OperandType::DataRegister: [[fallthrough]];
        case OperandType::AddressRegister:
            return instr.operands[num].value & 0b111;

        default:
            return utils::to_val(instr.operands[num].specialAddressingMode);
        }
    }

    constexpr std::array<std::uint8_t, 3> move_sizeconv = {
        0b01, // Byte (DataType::Byte)
        0b11, // Word (DataType::Word)
        0b10  // Long (DataType::Long)
    };

    constexpr std::array<std::uint8_t, 4> move_tobyte = {
        0, // ...
        1, // Byte
        4, // Long
        2, // Word
    };

    constexpr std::array<std::uint8_t, 3> tobyte = {
        1, // ...
        2, // Byte
        4, // Word
    };
}
