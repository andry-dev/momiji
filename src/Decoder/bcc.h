#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction bcc(gsl::span<std::uint16_t> mem, int idx);
}
