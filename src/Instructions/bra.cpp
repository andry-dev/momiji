#include "bra.h"

namespace momiji::instr
{
    momiji::System bra(momiji::System sys, const InstructionData& data)
    {
        sys.cpu.programCounter.address += utils::to_val(data.op1);
        return sys;
    }
}
