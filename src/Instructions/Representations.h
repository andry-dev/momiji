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
                , dst{0}, dstmode{0}
                , srcmode{0}, src{0}
            {}

            std::uint16_t header : 2;
            std::uint16_t size : 2;
            std::uint16_t dst : 3;
            std::uint16_t dstmode : 3;
            std::uint16_t srcmode : 3;
            std::uint16_t src : 3;
        };

        struct MoveA
        {
        };

        struct Add
        {
            Add()
                : header{0b1101}
                , size{0}
                , datareg{0}
                , direction{0}
                , othmode{0}, oth{0}
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
            std::uint16_t othmode : 3;

            // Source operand type
            std::uint16_t oth : 3;
        };

        struct AddA
        {
            AddA()
                : header{0b1101}
                , addreg{0}
                , size{0}
                , padding{0b11}
                , srcmode{0}, src{0}
            {}

            // 0b1101
            std::uint16_t header : 4;

            std::uint16_t addreg : 3;

            // Word or Long
            std::uint16_t size : 1;

            // Always 0b11
            std::uint16_t padding : 2;

            std::uint16_t srcmode : 3;
            std::uint16_t src : 3;
        };

        struct AddI
        {
            AddI()
                : header{0b00000110}
                , size{0}
                , dstmode{0}
                , dst{0}
            {}

            // 0b00000110
            std::uint16_t header : 8;

            // Byte, Word, Long
            std::uint16_t size : 2;

            std::uint16_t dstmode : 3;
            std::uint16_t dst : 3;
        };

        struct Sub
        {
            Sub()
                : header{0b1001}
                , datareg{0}
                , direction{0}
                , size{0}
                , othmode{0}
                , oth{0}
            {}

            std::uint16_t header : 4;
            std::uint16_t datareg : 3;
            std::uint16_t direction : 1;
            std::uint16_t size : 2;
            std::uint16_t othmode : 3;
            std::uint16_t oth : 3;
        };

        struct SubA
        {
            SubA()
                : header{0b1001}
                , addreg{0}
                , size{0}
                , padding{0b11}
                , srcmode{0}
                , src{0}
            {}

            std::uint16_t header : 4;
            std::uint16_t addreg : 3;
            std::uint16_t size : 1;
            std::uint16_t padding : 2;
            std::uint16_t srcmode : 3;
            std::uint16_t src : 3;
        };

        struct SubI
        {
            SubI()
                : header{0b00000100}
            {}

            std::uint16_t header : 8;
            std::uint16_t size : 2;
            std::uint16_t dstmode : 2;
            std::uint16_t dst : 2;
        };


        static_assert(sizeof(Move) == 2, "'move' is not 16-bit long");
        static_assert(sizeof(Add) == 2, "'add' is not 16-bit long");
        static_assert(sizeof(AddI) == 2, "'addi' is not 16-bit long");
        static_assert(sizeof(AddA) == 2, "'adda' is not 16-bit long");
        static_assert(sizeof(Sub) == 2, "'sub' is not 16-bit long");
        static_assert(sizeof(SubA) == 2, "'suba' is not 16-bit long");
        static_assert(sizeof(SubI) == 2, "'suba' is not 16-bit long");
    }
}
