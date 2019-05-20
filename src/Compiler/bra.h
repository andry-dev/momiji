#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void bra(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::Bra bits;

        std::uint16_t tmp = instr.operands[0].value;
        if (tmp > 255)
        {
            bits.displacement = 0;

            additionalData.val = instr.operands[0].value;
            additionalData.cnt = 2;
        }
        else
        {
            bits.displacement = instr.operands[0].value & 0x000000FF;
        }

        opcode.val =  (bits.header << 8)
                    | (bits.displacement);
    }
}
