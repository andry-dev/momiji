#pragma once

#include <cstdint>
#include <cstddef>

namespace momiji
{
    namespace repr
    {
        template <typename T>
        constexpr std::uint16_t to_val(T& rapr)
        {
            return *reinterpret_cast<std::uint16_t*>(&rapr);
        }

        struct Move
        {
            Move()
                : header{0b00}, size{0}
            {}

            std::uint16_t header : 2;
            std::uint16_t size : 2;
            std::uint16_t dstmode : 3;
            std::uint16_t dsttype : 3;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct MoveA
        {
        };

        struct MoveQ
        {
        };

        struct Add
        {
            Add()
                : header{0b1101}
            {}
            // 0b1101
            std::uint16_t header : 4;

            // Data register
            std::uint16_t datareg : 3;

            // 0 = Dn + EA -> Dn ; add.* (a0), d0
            // 1 = EA + Dn -> EA ; add.* d0, (a2)
            std::uint16_t direction : 1; 

            // Instr size (b, w, l)
            std::uint16_t size : 2;

            // Addressing mode
            std::uint16_t othtype : 3;

            // Source operand type
            std::uint16_t othmode : 3;
        };

        struct AddA
        {
            AddA()
                : header{0b1101}
            {}

            // 0b1101
            std::uint16_t header : 4;

            std::uint16_t addreg : 3;

            // Word or Long
            std::uint16_t size : 1;

            // Always 0b11
            std::uint16_t padding : 2;

            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct AddI
        {
            AddI()
                : header{0b00000110}
            {}

            // 0b00000110
            std::uint16_t header : 8;

            // Byte, Word, Long
            std::uint16_t size : 2;

            std::uint16_t dsttype : 3;
            std::uint16_t dstmode : 3;
        };

        struct AddQ
        {
        };

        struct Sub
        {
            Sub()
                : header{0b1001}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t direction : 1;
            std::uint16_t size : 2;
            std::uint16_t othtype : 3;
            std::uint16_t othmode : 3;
        };

        struct SubA
        {
            SubA()
                : header{0b1001}
                , padding{0b11}
            {}

            std::uint16_t header : 4;
            std::uint16_t addreg : 3;
            std::uint16_t size : 1;
            std::uint16_t padding : 2;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct SubI
        {
            SubI()
                : header{0b00000100}
            {}

            std::uint16_t header : 8;
            std::uint16_t size : 2;
            std::uint16_t dsttype : 2;
            std::uint16_t dstmode : 2;
        };

        struct SubQ
        {
        };

        struct DivS
        {
            DivS()
                : header{0b1000}
                , padding{0b111}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t padding : 3;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct DivU
        {
            DivU()
                : header{0b1000}
                , padding{0b011}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t padding : 3;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct MulS
        {
            MulS()
                : header{0b1100}
                , padding{0b111}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t padding : 3;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct MulU
        {
            MulU()
                : header{0b1100}
                , padding{0b011}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t padding : 3;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct Or
        {
            Or()
                : header{0b1000}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;

            // 0: or.* *, d*
            // 1: or.* d*, *
            std::uint16_t direction : 1;

            std::uint16_t size : 2;
            std::uint16_t othtype : 3;
            std::uint16_t othmode : 3;
        };

        struct OrI
        {
            OrI()
                : header{0b00000000}
            {}

            std::uint16_t header : 8;
            std::uint16_t size : 2;
            std::uint16_t dsttype : 3;
            std::uint16_t dstmode : 3;
        };

        struct And
        {
            And()
                : header{0b1100}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t direction : 1;
            std::uint16_t size : 2;
            std::uint16_t othtype : 3;
            std::uint16_t othmode : 3;
        };

        struct AndI
        {
            AndI()
                : header{0b00000010}
            {}

            std::uint16_t header : 8;
            std::uint16_t size : 2;
            std::uint16_t dsttype : 3;
            std::uint16_t dstmode : 3;
        };

        // Also known as EOR
        struct Xor
        {

        };

        // Also known as EORI
        struct XorI
        {

        };

        struct Not
        {
        };

        struct Swap
        {
        };

        struct Exg
        {
        };

        struct Cmp
        {
            Cmp()
                : header{0b1011}
                , padding{0b0}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t padding : 1;
            std::uint16_t size : 2;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct CmpA
        {
            CmpA()
                : header{0b1011}
                , padding{0b011}
            {}

            std::uint16_t header : 4;
            std::uint16_t addreg : 3;
            std::uint16_t size : 1;
            std::uint16_t padding : 2;
            std::uint16_t srctype : 3;
            std::uint16_t srcmode : 3;
        };

        struct CmpI
        {
            CmpI()
                : header{0b00001100}
            {}

            std::uint16_t header : 8;
            std::uint16_t size : 2;
            std::uint16_t dsttype : 3;
            std::uint16_t dstmode : 3;
        };

        struct Tst
        {
        };

        struct Bra
        {
        };

        // Conditional branch
        struct Bcc
        {
        };

        struct Jmp
        {
        };


        struct Illegal
        {
            Illegal()
                : value{0b01001010'11111100}
            {}

            std::uint16_t value;
        };

        struct Nop
        {
            Nop()
                : value{0b01001110'01110001}
            {}

            std::uint16_t value;
        };

        static_assert(sizeof(Move) == 2, "'move' is not 16-bit long");
        static_assert(sizeof(Add) == 2, "'add' is not 16-bit long");
        static_assert(sizeof(AddI) == 2, "'addi' is not 16-bit long");
        static_assert(sizeof(AddA) == 2, "'adda' is not 16-bit long");
        static_assert(sizeof(Sub) == 2, "'sub' is not 16-bit long");
        static_assert(sizeof(SubA) == 2, "'suba' is not 16-bit long");
        static_assert(sizeof(SubI) == 2, "'subi' is not 16-bit long");
        static_assert(sizeof(DivS) == 2, "'divs' is not 16-bit long");
        static_assert(sizeof(DivU) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(Or) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(OrI) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(Cmp) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(CmpI) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(CmpA) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(Illegal) == 2, "'divu' is not 16-bit long");
        static_assert(sizeof(Nop) == 2, "'divu' is not 16-bit long");
    }
}
