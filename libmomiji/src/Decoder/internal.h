#pragma once

#include "./Utils.h"
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction momijiInternal(ExecutableMemoryView mem,
                                      std::int64_t idx);
}
