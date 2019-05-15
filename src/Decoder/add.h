#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction add(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction adda(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction addi(gsl::span<std::uint16_t> mem, int idx);
    }
}
