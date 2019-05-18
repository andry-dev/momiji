#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction exg(gsl::span<std::uint16_t> mem, int idx);
}
