#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction exg(ExecutableMemoryView mem, std::uint64_t idx);
}
