#include "bra.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System bra(momiji::System sys, const InstructionData& data)
    {
        std::int16_t offset = utils::to_val(data.op1);

        auto* pc = sys.cpu.programCounter.address;

        if (offset == 0)
        {
            offset = *(pc + 1);
        }

        sys.cpu.programCounter.address += offset;

        return sys;
    }
}
