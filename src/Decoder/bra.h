#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction bra(ExecutableMemoryView mem, std::uint64_t idx);
}
