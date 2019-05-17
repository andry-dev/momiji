#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction muls(gsl::span<std::uint16_t> mem, int idx);
    DecodedInstruction mulu(gsl::span<std::uint16_t> mem, int idx);
}
