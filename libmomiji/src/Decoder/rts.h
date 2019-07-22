#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction rts(ExecutableMemoryView mem, std::uint64_t idx);
}
