#include "noop.h"

namespace momiji::instr
{
    momiji::System noop(momiji::System& sys, const InstructionData& /*data*/)
    {
        return sys;
    }
} // namespace momiji::instr
