#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction and_ins(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction andi(gsl::span<std::uint16_t> mem, int idx);
    }
}

