#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction divs(gsl::span<std::uint16_t> mem, int idx);
    DecodedInstruction divu(gsl::span<std::uint16_t> mem, int idx);
}
