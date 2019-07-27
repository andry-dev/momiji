#include "rod.h"

#include "../Instructions/Representations.h"
#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction mem_rod(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        ret.string = "rod ";

        return ret;
    }

    DecodedInstruction reg_rod(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        ret.string = "rod ";

        return ret;
    }
} // namespace momiji::dec
