#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction tst(ExecutableMemoryView mem, std::int64_t idx);
}
