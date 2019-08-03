#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction rts(ExecutableMemoryView mem, std::int64_t idx);
}
