#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction swap(ExecutableMemoryView mem, std::int64_t idx);
}
