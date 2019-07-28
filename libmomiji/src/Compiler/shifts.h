#pragma once

#include <momiji/Compiler.h>

#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <iostream>

namespace momiji::enc
{
    template <typename RegRepr, typename MemRepr>
    void any_shift(const Instruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData,
                   int dirBit)
    {
        // Reg
        if (discriminateShifts(instr))
        {
            RegRepr bits;

            bits.direction = dirBit;
            bits.rotation  = instr.operands[0].value;

            if (instr.operands[0].operandType == OperandType::DataRegister)
            {
                bits.rotmode = 1;
            }
            else
            {
                bits.rotmode = 0;
            }

            bits.datareg = instr.operands[1].value;
            bits.size    = utils::to_val(instr.dataType) & 0b111;

            opcode.val = (bits.header << 12) | (bits.rotation << 9) |
                         (bits.direction << 8) | (bits.size << 6) |
                         (bits.rotmode << 5) | (bits.padding << 3) |
                         (bits.datareg);
        }
        // Mem
        else
        {
            MemRepr bits;

            bits.direction = dirBit;

            bits.regtype = utils::to_val(instr.operands[0].operandType);
            bits.regmode =
                utils::to_val(instr.operands[0].specialAddressingMode);

            opcode.val = (bits.header << 9) | (bits.direction << 8) |
                         (bits.padding << 6) | (bits.regtype << 3) |
                         (bits.regmode);
        }
    }
} // namespace momiji::enc
