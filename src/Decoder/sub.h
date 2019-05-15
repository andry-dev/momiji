#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction sub(gsl::span<std::uint16_t> mem, int idx);

        DecodedInstruction suba(gsl::span<std::uint16_t> mem, int idx);

        DecodedInstruction subi(gsl::span<std::uint16_t> mem, int idx);
    }
}
