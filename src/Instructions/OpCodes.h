#pragma once

#include <cstddef>
#include <cstdint>

namespace momiji
{
    namespace codes
    {
        // 00 2*SIZE 6*EA 6*EA
        constexpr std::uint16_t move = 0b00'00'000000'000000;

        // 00 2*SIZE 3*DST 001 6*EA
        constexpr std::uint16_t movea = 0b00'00'000'001'000000;

        // 0111 3*DST 0 8*DATA
        constexpr std::uint16_t moveq = 0b0111'000'0'00000000;

        // 1101 3*DST 1*OPMODE 2*SIZE 6*EA
        constexpr std::uint16_t add = 0b1101'000'0'00'000000;

        // 1101 3*DST 1*OPMODE 11 6*EA
        constexpr std::uint16_t adda = 0b1101'000'0'11'000000;

        // 00000 110 2*SIZE 6*EA
        constexpr std::uint16_t addi = 0b00000'110'00'000000;

        // 0101 3*DATA 0 2*SIZE 6*EA
        constexpr std::uint16_t addq = 0b0101'000'0'00'000000;

        // 1001 3*DST 3*OPMODE 6*EA
        constexpr std::uint16_t sub = 0b1001'000'000'000000;
        constexpr std::uint16_t suba = sub;

        // 00000 100 2*SIZE 6*EA
        constexpr std::uint16_t subi = 0b00000'100'00'000000;

        // 0101 3*DATA 1 2*SIZE 6*EA
        constexpr std::uint16_t subq = 0b0101'000'1'00'000000;

        // 1100 3*DST 111 6*EA
        constexpr std::uint16_t muls = 0b1100'000'111'000000;

        // 1100 3*DST 011 6*EA
        constexpr std::uint16_t mulu = 0b1100'000'011'000000;

        // 1000 3*DST 111 6*EA
        constexpr std::uint16_t divs = 0b1000'000'111'000000;

        // 1000 3*DST 011 6*EA
        constexpr std::uint16_t divu = 0b1000'000'011'000000;

        // 010010000100 3*REG
        constexpr std::uint16_t swap = 0b010010000100'000;

        // 01001010 2*SIZE 6*EA
        constexpr std::uint16_t tst = 0b01001010'00'000000;

        // 1011 3*DST 3*OPMODE 6*EA
        constexpr std::uint16_t cmp = 0b1011'000'000'000000;
        constexpr std::uint16_t cmpa = cmp;

        // 00001100 2*SIZE 6*EA
        constexpr std::uint16_t cmpi = 0b00001100'00'000000;

        // 1011 3*DST 1 2*SIZE 001 3*SRC
        constexpr std::uint16_t cmpm = 0b1011'000'1'00'001'000;

        // 0110 4*CND 8*DISPLACEMENT
        constexpr std::uint16_t bcc = 0b0110'0000'00000000;

        // 01100000 8*DISPLACEMENT
        constexpr std::uint16_t bra = 0b01100000'00000000;

        // 0100111011 6*EA
        constexpr std::uint16_t jmp = 0b0100111011'000000;
    } // namespace codes
} // namespace momiji
