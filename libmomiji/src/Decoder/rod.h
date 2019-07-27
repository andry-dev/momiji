#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction mem_rod(ExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction reg_rod(ExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
