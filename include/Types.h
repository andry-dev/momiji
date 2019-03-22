#pragma once

#include <cstdint>

namespace momiji
{
    enum class instruction_type : std::int8_t
    {
        Add, // add.*
        Sub, // sub.*

        SignedMul,   // muls
        UnsignedMul, // mulu
        SignedDiv,   // divs
        UnsignedDiv, // divu

        Swap,     // swap
        Exchange, // exg

        Move, // move.*
        MoveImmediate,  // moveq
    };

    enum class operand_type : std::int8_t
    {
        Immediate,
        Register,
    };

    enum class register_type : std::int8_t
    {
        Data,
        Address,
        Special,
    };

    enum class data_type : std::int8_t
    {
        Long, // 32
        Word, // 16
        Byte,  // 8
        Address,
    };
}
