#include <Decoder.h>

#include "../Instructions/add.h"
#include "../Instructions/illegal.h"
#include "../Instructions/noop.h"
#include "../Instructions/sub.h"

#include "add.h"
#include "and.h"
#include "bcc.h"
#include "bra.h"
#include "cmp.h"
#include "div.h"
#include "exg.h"
#include "internal.h"
#include "jmp.h"
#include "move.h"
#include "mul.h"
#include "not.h"
#include "or.h"
#include "rod.h"
#include "roxd.h"
#include "rts.h"
#include "shifts.h"
#include "sub.h"
#include "swap.h"
#include "tst.h"

namespace momiji
{
    DecodedInstruction::DecodedInstruction()
        : exec(momiji::instr::illegal)
    {
    }

    DecodedInstruction decodeFirstGroup(ExecutableMemoryView mem,
                                        std::int64_t idx);
    DecodedInstruction decodeSecondGroup(ExecutableMemoryView mem,
                                         std::int64_t idx);
    DecodedInstruction decodeThirdGroup(ExecutableMemoryView mem,
                                        std::int64_t idx);
    DecodedInstruction decodeFourthGroup(ExecutableMemoryView mem,
                                         std::int64_t idx);

    DecodedInstruction decode(ExecutableMemoryView mem, std::int64_t idx)
    {
        // Find by groups
        std::uint16_t mask = 0b11000000'00000000;

        const std::uint16_t val = mem.read16(idx) & mask;

        switch (val)
        {
        case 0b00000000'00000000:
            return decodeFirstGroup(mem, idx);

        case 0b01000000'00000000:
            return decodeSecondGroup(mem, idx);

        case 0b10000000'00000000:
            return decodeThirdGroup(mem, idx);

        case 0b11000000'00000000:
            return decodeFourthGroup(mem, idx);
        }

        return {};
    }

    DecodedInstruction decodeFirstGroup(ExecutableMemoryView mem,
                                        std::int64_t idx)
    {
        constexpr std::uint16_t firstmask  = 0b11110000'00000000;
        constexpr std::uint16_t secondmask = 0b11111111'00000000;

        switch (mem.read16(idx) & firstmask)
        {
        case 0b00000000'00000000:
            switch (mem.read16(idx) & secondmask)
            {
            // ANDI
            case 0b00000010'00000000:
                return momiji::dec::andi(mem, idx);

            // SUBI
            case 0b00000100'00000000:
                return momiji::dec::subi(mem, idx);

            // ADDI
            case 0b00000110'00000000:
                return momiji::dec::addi(mem, idx);

            // CMPI
            case 0b00001100'00000000:
                return momiji::dec::cmpi(mem, idx);
            }
            break;

        // MOVE / MOVEA
        case 0b00010000'00000000:
        case 0b00110000'00000000:
        case 0b00100000'00000000:
            return momiji::dec::move(mem, idx);
        }

        return {};
    }

    DecodedInstruction decodeSecondGroup(ExecutableMemoryView mem,
                                         std::int64_t idx)
    {
        constexpr std::uint16_t firstmask  = 0b11110000'00000000;
        constexpr std::uint16_t bramask    = 0b11111111'00000000;
        constexpr std::uint16_t secondmask = 0b11111111'11000000;

        const std::uint16_t val = mem.read16(idx);

        switch (val & firstmask)
        {
        // JMP / MOVEM
        case 0b01000000'00000000:
            switch (val & secondmask)
            {
            // TST
            case 0b01001010'00000000:
            case 0b01001010'01000000:
            case 0b01001010'10000000:
                return momiji::dec::tst(mem, idx);

            // NOT
            case 0b01000110'00000000:
            case 0b01000110'01000000:
            case 0b01000110'10000000:
                // return momiji::dec::not_instr(mem, idx);
                break;

            // SWAP / PEA
            case 0b01001000'01000000:
                return momiji::dec::swap(mem, idx);

            // RTE / RTS / TRAPV / RTR
            case 0b01001110'01000000:
                switch (val)
                {
                // RTS
                case 0b01001110'01110101:
                    return momiji::dec::rts(mem, idx);
                }
                break;

            // JSR
            case 0b01001110'10000000:
                return momiji::dec::jsr(mem, idx);

            // JMP
            case 0b01001110'11000000:
                return momiji::dec::jmp(mem, idx);
            }
            break;

        // ADDQ / SUBQ / Scc / DBcc
        case 0b01010000'00000000:
            break;

        // BRA / BSR / Bcc
        case 0b01100000'00000000:
            switch (mem.read16(idx) & bramask)
            {
            // BRA
            case 0b01100000'00000000:
                return momiji::dec::bra(mem, idx);

            // BSR
            case 0b01100001'00000000:
                return momiji::dec::bsr(mem, idx);

            // Bcc, probably
            default:
                return momiji::dec::bcc(mem, idx);
            }

        // MOVEQ
        case 0b011100000'00000000:
            // return momiji::dec::moveq(mem, idx);
            break;
        }

        return {};
    }

    DecodedInstruction decodeThirdGroup(ExecutableMemoryView mem,
                                        std::int64_t idx)
    {
        constexpr std::uint16_t firstmask = 0b11110000'11000000;
        constexpr std::uint16_t divmask   = 0b11110001'11000000;

        const std::uint16_t val = mem.read16(idx);

        switch (val & firstmask)
        {
        // DIVU / DIVS
        case 0b10000000'11000000:
            switch (val & divmask)
            {
            // DIVU
            case 0b10000000'11000000:
                return momiji::dec::divu(mem, idx);

            // DIVS
            case 0b10000001'11000000:
                return momiji::dec::divs(mem, idx);
            }
            break;

        // OR
        case 0b10000000'00000000:
        case 0b10000000'01000000:
        case 0b10000000'10000000:
            return momiji::dec::or_instr(mem, idx);

        // SUB
        case 0b10010000'00000000:
        case 0b10010000'01000000:
        case 0b10010000'10000000:
            return momiji::dec::sub(mem, idx);

        // SUBA
        case 0b10010000'11000000:
            return momiji::dec::suba(mem, idx);

        // CMP
        case 0b10110000'00000000:
        case 0b10110000'01000000:
        case 0b10110000'10000000:
            return momiji::dec::cmp(mem, idx);

        // CMPA
        case 0b10110000'11000000:
            return momiji::dec::cmpa(mem, idx);
        }

        return {};
    }

    DecodedInstruction decodeFourthGroup(ExecutableMemoryView mem,
                                         std::int64_t idx)
    {
        // Momiji specific control code!
        if (mem.read16(idx) == 0xFFFF)
        {
            return momiji::dec::momijiInternal(mem, idx);
        }

        constexpr std::uint16_t firstmask = 0b11110000'11000000;
        constexpr std::uint16_t mulmask   = 0b11110001'11000000;

        // Used to discriminate between AND or EXG
        constexpr std::uint16_t exgmask = 0b11110001'11001000;

        constexpr std::uint16_t memshiftmask = 0b11111111'11000000;
        constexpr std::uint16_t regshiftmask = 0b11110001'00111000;

        const auto val = mem.read16(idx);

        switch (val & firstmask)
        {
        // MULU / MULS
        case 0b11000000'11000000:
            switch (val & mulmask)
            {
            // MULU
            case 0b11000000'11000000:
                return momiji::dec::mulu(mem, idx);

            case 0b11000001'11000000:
                return momiji::dec::muls(mem, idx);
            }
            break;

        // AND / EXG
        case 0b11000000'00000000:
        case 0b11000000'01000000:
        case 0b11000000'10000000:
            switch (val & exgmask)
            {
            // Probably an EXG
            case 0b11000001'01000000:
            case 0b11000001'01001000:
            case 0b11000001'10001000:
                return momiji::dec::exg(mem, idx);

            // Hoping for an AND
            default:
                return momiji::dec::and_instr(mem, idx);
            }

        // ADD
        case 0b11010000'00000000:
        case 0b11010000'01000000:
        case 0b11010000'10000000:
            return momiji::dec::add(mem, idx);

        // ADDA
        case 0b11010000'11000000:
            return momiji::dec::adda(mem, idx);
        }

        // Memory Shifts
        switch (val & memshiftmask)
        {
        // AS* (mem)
        case 0b11100000'11000000:
        case 0b11100001'11000000:
            return momiji::dec::any_mem_shift<
                dec::details::ArithmeticShiftMetaType>(mem, idx);

        // LS* (mem)
        case 0b11100010'11000000:
        case 0b11100011'11000000:
            return momiji::dec::any_mem_shift<
                dec::details::LogicalShiftMetaType>(mem, idx);

        // ROX* (mem)
        case 0b11100100'11000000:
        case 0b11100101'11000000:
            // return momiji::dec::mem_roxd(mem, idx);

        // RO* (mem)
        case 0b11100110'11000000:
        case 0b11100111'11000000:
            // return momiji::dec::mem_rod(mem, idx);
            break;
        }

        switch (val & regshiftmask)
        {
        // AS* (reg)
        case 0b11100000'00000000:
        case 0b11100000'00100000:
        case 0b11100001'00000000:
        case 0b11100001'00100000:
            return momiji::dec::any_reg_shift<
                dec::details::ArithmeticShiftMetaType>(mem, idx);

        // LS* (reg)
        case 0b11100000'00001000:
        case 0b11100000'00101000:
        case 0b11100001'00001000:
        case 0b11100001'00101000:
            return momiji::dec::any_reg_shift<
                dec::details::LogicalShiftMetaType>(mem, idx);

        // ROX* (reg)
        case 0b11100000'00010000:
        case 0b11100000'00110000:
        case 0b11100001'00010000:
        case 0b11100001'00110000:
            // return momiji::dec::reg_roxd(mem, idx);

        case 0b11100000'00011000:
        case 0b11100000'00111000:
        case 0b11100001'00011000:
        case 0b11100001'00111000:
            // return momiji::dec::reg_rod(mem, idx);
            break;
        }

        return {};
    }
} // namespace momiji
