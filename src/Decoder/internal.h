#pragma once

#include "./Utils.h"
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction momijiInternal(gsl::span<std::uint16_t> mem, int idx);
}
