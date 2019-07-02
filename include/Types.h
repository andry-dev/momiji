#pragma once

#include <cstdint>
#include <vector>

namespace momiji
{
    enum class InstructionType : std::uint8_t
    {
        Add,  // add.*
        AddI, // addi.*
        AddA, // adda.*
        Sub,  // sub.*
        SubI, // subi.*
        SubA, // suba.*

        SignedMul,   // muls
        UnsignedMul, // mulu
        SignedDiv,   // divs
        UnsignedDiv, // divu

        Swap,     // swap
        Exchange, // exg

        Move,        // move.*
        MoveQuick,   // moveq
        MoveAddress, // movea

        Or,
        OrI,
        And,
        AndI,
        Xor,
        XorI,
        Not,
        Neg,

        Compare,
        CompareI,
        CompareA,
        Tst,

        Jmp,
        Branch,
        BranchCondition,

        ArithmeticShiftLeft,
        ArithmeticShiftRight,

        LogicalShiftLeft,
        LogicalShiftRight,

        Nop,
        Illegal
    };

    enum class OperandType : std::uint8_t
    {
        DataRegister = 0b000,
        AddressRegister = 0b001,
        Address = 0b010,
        AddressPost = 0b011,
        AddressPre = 0b100,
        AddressOffset = 0b101,
        AddressIndex = 0b110,
        ProgramCounterOffset = 0b111,
        ProgramCounterIndex = 0b111,
        AbsoluteShort = 0b111,
        AbsoluteLong = 0b111,
        Immediate = 0b111,
    };

    enum class DataType : std::uint8_t
    {
        Byte = 0b00, // 8
        Word = 0b01, // 16
        Long = 0b10, // 32
    };

    enum class SpecialAddressingMode : std::uint8_t
    {
        ProgramCounterOffset = 0b010,
        ProgramCounterIndex = 0b011,
        AbsoluteShort = 0b000,
        AbsoluteLong = 0b001,
        Immediate = 0b100,
    };

    enum class BranchConditions : std::uint8_t
    {
        True = 0b0000,
        False = 0b0001,
        Higher = 0b0010,
        LowerSame = 0b0011,
        CarryClear = 0b0100,
        CarrySet = 0b0101,
        NotEqual = 0b0110,
        Equal = 0b0111,
        OverClear = 0b1000,
        OverSet = 0b1001,
        Plus = 0b1010,
        Minus = 0b1011,
        GreaterEq = 0b1100,
        LessThan = 0b1101,
        GreaterThan = 0b1110,
        LessEq = 0b1111,
    };

    using MemoryType = std::vector<std::uint16_t>;
} // namespace momiji
