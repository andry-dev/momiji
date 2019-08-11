#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include <momiji/Parser.h>

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

#if 0
    namespace v1
    {

        std::uint8_t getCorrectOpMode(const v1::Instruction& instr,
                                      std::int8_t op);

        void
        handleAdditionalData(const v1::Instruction& instr,
                             std::array<AdditionalData, 2>& additionalData);

        bool discriminateShifts(const Instruction& instr);
    } // namespace v1
#endif

    inline namespace v2
    {

        std::uint8_t getCorrectOpMode(const v2::ParsedInstruction& instr,
                                      std::int8_t opNum);

        void
        handleAdditionalData(const v2::ParsedInstruction& instr,
                             const v2::LabelInfo& labels,
                             std::array<AdditionalData, 2>& additionalData);

        bool discriminateShifts(const v2::ParsedInstruction& instr);
    } // namespace v2

} // namespace momiji
