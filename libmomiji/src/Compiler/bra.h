#pragma once

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void bra(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Bra bits;

#ifdef LIBMOMIJI_CORRECT_BRA_IMPL
        std::uint16_t tmp = instr.operands[0].value;
        if (tmp > 255)
        {
            bits.displacement = 0;

            additionalData[0].val = instr.operands[0].value;
            additionalData[0].cnt = 2;
        }
        else
        {
            bits.displacement = instr.operands[0].value & 0x000000FF;
        }
#else
        bits.displacement = 0;

        additionalData[0].val = instr.operands[0].value;
        additionalData[0].cnt = 2;
#endif

        opcode.val = (bits.header << 8) | (bits.displacement);
    }

    void bsr(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Bsr bits;

#ifdef LIBMOMIJI_CORRECT_BRA_IMPL
        std::uint16_t tmp = instr.operands[0].value;
        if (tmp > 255)
        {
            bits.displacement = 0;

            additionalData[0].val = instr.operands[0].value;
            additionalData[0].cnt = 2;
        }
        else
        {
            bits.displacement = instr.operands[0].value & 0x000000FF;
        }
#else
        bits.displacement     = 0;

        additionalData[0].val = instr.operands[0].value;
        additionalData[0].cnt = 2;
#endif

        opcode.val = (bits.header << 8) | (bits.displacement);
    }
} // namespace momiji
