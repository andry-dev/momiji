#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction or_instr(gsl::span<std::uint16_t> mem, int idx);
    DecodedInstruction ori(gsl::span<std::uint16_t> mem, int idx);
    DecodedInstruction ora(gsl::span<std::uint16_t> mem, int idx);
}
