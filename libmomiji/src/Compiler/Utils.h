#pragma once

#include <Memory.h>
#include <Parser.h>
#include <Utils.h>
#include <array>
#include <cstddef>
#include <cstdint>

namespace momiji
{
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

    std::uint8_t getCorrectOpMode(const momiji::Instruction& instr, int op)
    {
        switch (instr.operands[op].operandType)
        {
        case OperandType::DataRegister:
            [[fallthrough]];
        case OperandType::AddressRegister:
            [[fallthrough]];
        case OperandType::Address:
            [[fallthrough]];
        case OperandType::AddressPost:
            [[fallthrough]];
        case OperandType::AddressPre:
            [[fallthrough]];
        case OperandType::AddressIndex:
            [[fallthrough]];
        case OperandType::AddressOffset:
            return instr.operands[op].value & 0b111;

        default:
            return utils::to_val(instr.operands[op].specialAddressingMode);
        }
    }

    void handleAdditionalData(const momiji::Instruction& instr,
                              std::array<AdditionalData, 2>& additionalData)
    {
        Expects(instr.operands.size() <= 2,
                "How could you even get more than 2 operands here?");

        auto size = utils::to_val(instr.dataType);

        for (int i = 0; i < instr.operands.size(); ++i)
        {
            switch (instr.operands[i].operandType)
            {
            // offset(a*)
            case OperandType::AddressIndex:
                [[fallthrough]];
            // (offset, a*, X*)
            case OperandType::AddressOffset:
                additionalData[i].cnt = 2;
                additionalData[i].val =
                    (instr.operands[i].value & 0xFFFF'0000) >> 16;
                break;

            case OperandType::Immediate:
                switch (instr.operands[i].specialAddressingMode)
                {
                // #1234
                case SpecialAddressingMode::Immediate:
                    additionalData[i].cnt = tobyte[size];
                    additionalData[i].val = instr.operands[i].value;
                    break;

                // *.w 1234
                case SpecialAddressingMode::AbsoluteShort:
                    additionalData[i].cnt = 2;
                    additionalData[i].val = instr.operands[i].value;
                    break;

                // *.l 1234
                case SpecialAddressingMode::AbsoluteLong:
                    additionalData[i].cnt = 4;
                    additionalData[i].val = instr.operands[i].value;
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    [[fallthrough]];
                case SpecialAddressingMode::ProgramCounterIndex:
                    // TODO(andry): Implement PC addressing
                    break;
                }
                break;

            default:
                break;
            }
        }
    }

    inline bool discriminateShifts(const Instruction& instr)
    {
        if ((instr.operands[0].operandType != OperandType::DataRegister) &&
            ((instr.operands[0].operandType != OperandType::Immediate) &&
             (instr.operands[0].specialAddressingMode !=
              SpecialAddressingMode::Immediate)))
        {
            return false;
        }

        return true;
    }
} // namespace momiji
