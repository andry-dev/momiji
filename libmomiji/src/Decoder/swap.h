#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction swap(ConstExecutableMemoryView mem, std::int64_t idx);
}
