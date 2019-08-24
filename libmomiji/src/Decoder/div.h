#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction divs(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction divu(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
