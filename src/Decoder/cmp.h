#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction cmp(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction cmpi(gsl::span<std::uint16_t> mem, int idx);
        DecodedInstruction cmpa(gsl::span<std::uint16_t> mem, int idx);
    }
}
