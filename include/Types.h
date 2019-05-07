#pragma once

#include <cstdint>

namespace momiji
{
    enum class InstructionType : std::int8_t
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

        Compare,

        Jmp,
        BranchEqual,
        BranchNotEquals,
        BranchGreaterThan,
        BranchLessThan,
        BranchGreaterEquals,
        BranchLessEquals,
        BranchZero,
        BranchNotZero,
    };

    enum class OperandType : std::int8_t
    {
        Immediate,
        Register,
        Address,
        Label,
    };

    enum class RegisterType : std::int8_t
    {
        Data,
        Address,
        Special,
    };

    enum class DataType : std::int8_t
    {
        Byte = 0,  // 8
        Word = 1, // 16
        Long = 2, // 32
        Address = 3,
    };
}
