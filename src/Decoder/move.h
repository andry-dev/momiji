#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction move(gsl::span<std::uint16_t> mem, int idx);
    }
}
