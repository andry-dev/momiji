#include <Decoder.h>

#include "../Instructions/add.h"
#include "../Instructions/sub.h"
#include "../Instructions/noop.h"
#include "../Instructions/illegal.h"

#include "move.h"
#include "add.h"
#include "sub.h"
#include "cmp.h"
#include "and.h"
#include "or.h"
#include "mul.h"
#include "div.h"
#include "exg.h"

namespace momiji
{
    DecodedInstruction::DecodedInstruction()
        : exec(momiji::instr::illegal)
    {

    }

    using MemoryView = gsl::span<std::uint16_t>;

    DecodedInstruction decodeFirstGroup(const MemoryView& mem, int idx);
    DecodedInstruction decodeSecondGroup(const MemoryView& mem, int idx);
    DecodedInstruction decodeThirdGroup(const MemoryView& mem, int idx);
    DecodedInstruction decodeFourthGroup(const MemoryView& mem, int idx);

    DecodedInstruction decode(MemoryView mem, int idx)
    {
        // Find by groups
        std::uint16_t mask = 0b11000000'00000000;

        std::uint16_t val = mem[idx] & mask;

        switch (val)
        {
        case 0b00000000'00000000:
            return decodeFirstGroup(mem, idx);

        case 0b01000000'00000000:
            //return decodeSecondGroup(mem);
            break;

        case 0b10000000'00000000:
            return decodeThirdGroup(mem, idx);

        case 0b11000000'00000000:
            return decodeFourthGroup(mem, idx);
        }

        return {};
    }


    DecodedInstruction decodeFirstGroup(const MemoryView& mem, int idx)
    {
        constexpr std::uint16_t firstmask = 0b11110000'00000000;
        constexpr std::uint16_t secondmask = 0b11111111'00000000;

        switch (mem[idx] & firstmask)
        {
        case 0b00000000'00000000:
            switch (mem[idx] & secondmask)
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

    DecodedInstruction decodeThirdGroup(const MemoryView& mem, int idx)
    {
        constexpr std::uint16_t firstmask = 0b11110000'11000000;
        constexpr std::uint16_t divmask = 0b11110001'11000000;

        switch (mem[idx] & firstmask)
        {
        // DIVU / DIVS
        case 0b10000000'11000000:
            switch (mem[idx] & divmask)
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


    DecodedInstruction decodeFourthGroup(const MemoryView& mem, int idx)
    {

        constexpr std::uint16_t firstmask = 0b11110000'11000000;
        constexpr std::uint16_t mulmask =   0b11110001'11000000;

        switch (mem[idx] & firstmask)
        {
        // MULU / MULS
        case 0b11000000'11000000:
            switch (mem[idx] & mulmask)
            {
            // MULU
            case 0b11000000'11000000:
                return momiji::dec::mulu(mem, idx);

            case 0b11000001'11000000:
                return momiji::dec::muls(mem, idx);
            }
            break;

        // AND
        case 0b11000000'00000000:
        case 0b11000000'01000000:
        case 0b11000000'10000000:
            return momiji::dec::and_instr(mem, idx);

        // EXG
        case 0b11000001'01000000:
        case 0b11000001'10000000:
            return momiji::dec::exg(mem, idx);

        // ADD
        case 0b11010000'00000000:
        case 0b11010000'01000000:
        case 0b11010000'10000000:
            return momiji::dec::add(mem, idx);

        // ADDA
        case 0b11010000'11000000:
            return momiji::dec::adda(mem, idx);
        }

        return {};
    }
}
