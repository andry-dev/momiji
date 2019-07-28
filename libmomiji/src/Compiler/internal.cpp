#include "internal.h"

#include <momiji/Utils.h>

namespace momiji::enc
{
    void hcf(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        additionalData[0].cnt = 2;
        additionalData[0].val = 0;

        opcode.val = 0xFFFF;
    }

    void breakpoint(const momiji::Instruction& instr,
                    OpcodeDescription& opcode,
                    std::array<AdditionalData, 2>& additionalData)
    {
        additionalData[0].cnt = 2;
        additionalData[0].val = 1;

        opcode.val = 0xFFFF;
    }
} // namespace momiji::enc
