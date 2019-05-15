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

namespace momiji
{
    DecodedInstruction::DecodedInstruction()
        : exec(momiji::instr::illegal)
    {

    }

    using MemoryView = gsl::span<std::uint16_t>;

    DecodedInstruction decodeFirstGroup(const MemoryView& mem, int idx);
    DecodedInstruction decodeSecondGroup(const MemoryType& mem);
    DecodedInstruction decodeThirdGroup(const MemoryType& mem);
    DecodedInstruction decodeFourthGroup(const MemoryType& mem);

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
        case 0b10000000'00000000:
            //return decodeThirdGroup(mem);
        case 0b11000000'00000000:
            //return decodeFourthGroup(mem);
            break;
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
}
