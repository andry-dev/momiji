#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction exg(ExecutableMemoryView mem, std::uint64_t idx);
}
