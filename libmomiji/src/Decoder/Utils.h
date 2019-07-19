#pragma once

#include <Decoder.h>

namespace momiji
{
    inline void assignNormalSize(DecodedInstruction& instr, std::uint8_t size)
    {
        switch (size)
        {
        case 0b00:
            instr.data.size = 1;
            break;

        case 0b01:
            instr.data.size = 2;
            break;

        case 0b10:
            instr.data.size = 4;
            break;
        }
    }
} // namespace momiji
