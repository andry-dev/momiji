#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace momiji
{
    enum class InstructionType : std::uint8_t
    {
        Add,  // add.*
        AddI, // addi.*
        AddA, // adda.*
        AddQ, // addq

        Sub,  // sub.*
        SubI, // subi.*
        SubA, // suba.*
        SubQ, // subq

        SignedMul,   // muls
        UnsignedMul, // mulu
        SignedDiv,   // divs
        UnsignedDiv, // divu

        Swap,     // swap
        Exchange, // exg

        Move,      // move.*
        MoveQuick, // moveq

        Or,  // or
        OrI, // ori

        And,  // and
        AndI, // andi

        Xor,  // eor
        XorI, // eori

        Not, // not
        Neg, // noti

        Compare,  // cmp
        CompareI, // cmpi
        CompareA, // cmpa

        Tst, // tst

        Jmp,           // jmp
        JmpSubroutine, // jsr

        Branch,           // bra
        BranchSubroutine, // bsr
        BranchCondition,  // bcc

        ReturnSubroutine, // rts

        ArithmeticShiftLeft,  // asl
        ArithmeticShiftRight, // asr
        LogicalShiftLeft,     // lsl
        LogicalShiftRight,    // lsr

        Nop, // nop
        Illegal,

        // Specific to momiji
        HaltCatchFire, // hcf
        Breakpoint,
        Declare, // dc.*
    };

    enum class OperandType : std::uint8_t
    {
        DataRegister         = 0b000, // d*
        AddressRegister      = 0b001, // a*
        Address              = 0b010, // (a*)
        AddressPost          = 0b011, // -(a*)
        AddressPre           = 0b100, // (a*)+
        AddressOffset        = 0b101, // num(a*)
        AddressIndex         = 0b110, // (num, a*, *)
        ProgramCounterOffset = 0b111,
        ProgramCounterIndex  = 0b111,
        AbsoluteShort        = 0b111,
        AbsoluteLong         = 0b111,
        Immediate            = 0b111,
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
        ProgramCounterIndex  = 0b011,
        AbsoluteShort        = 0b000,
        AbsoluteLong         = 0b001,
        Immediate            = 0b100,
    };

    enum class BranchConditions : std::uint8_t
    {
        True        = 0b0000,
        False       = 0b0001,
        Higher      = 0b0010,
        LowerSame   = 0b0011,
        CarryClear  = 0b0100,
        CarrySet    = 0b0101,
        NotEqual    = 0b0110,
        Equal       = 0b0111,
        OverClear   = 0b1000,
        OverSet     = 0b1001,
        Plus        = 0b1010,
        Minus       = 0b1011,
        GreaterEq   = 0b1100,
        LessThan    = 0b1101,
        GreaterThan = 0b1110,
        LessEq      = 0b1111,
    };

    using MemoryType = std::vector<std::uint16_t>;
} // namespace momiji
