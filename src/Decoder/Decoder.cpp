#include <Decoder.h>

#include "../Instructions/add.h"
#include "../Instructions/sub.h"
#include "../Instructions/noop.h"
#include "../Instructions/illegal.h"

#include "move.h"
#include "add.h"

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

        std::uint16_t mask = 0b11110000'00000000;

        std::uint16_t val = mem[idx];

        mask = val & mask;

        switch (mask)
        {
        case 0b00000000'00000000:
            break;
        case 0b00010000'00000000:
        case 0b00110000'00000000:
        case 0b00100000'00000000:
            return momiji::dec::move(mem, idx);
        }

        return {};
    }
}
