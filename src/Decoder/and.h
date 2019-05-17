#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction and_instr(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction andi(gsl::span<std::uint16_t> mem, int idx);
    }
}

