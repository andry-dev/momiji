#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction divs(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction divu(ExecutableMemoryView mem, std::uint64_t idx);
} // namespace momiji::dec
