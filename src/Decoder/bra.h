#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction bra(ExecutableMemoryView mem, std::uint64_t idx);
}
