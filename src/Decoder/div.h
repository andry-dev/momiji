#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction divs(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction divu(ExecutableMemoryView mem, std::uint64_t idx);
}
