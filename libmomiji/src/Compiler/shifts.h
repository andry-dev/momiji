#pragma once

#include <momiji/Compiler.h>

#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <momiji/Utils.h>

namespace momiji::enc
{
    template <typename RegRepr, typename MemRepr>
    void any_shift(const ParsedInstruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& /*additionalData*/,
                   std::uint8_t dirBit)
    {
        // Reg
        if (discriminateShifts(instr))
        {
            RegRepr bits;

            bits.direction = dirBit;
            bits.rotation  = std::uint16_t(instr.operands[0]);

            if (instr.operands[0].operandType == OperandType::DataRegister)
            {
                bits.rotmode = 1;
            }
            else
            {
                bits.rotmode = 0;
            }

            bits.datareg = std::uint16_t(instr.operands[1]);
            bits.size    = utils::to_val(instr.dataType) & 0b111;

            opcode.val = std::uint16_t(
                (bits.header << 12) | (bits.rotation << 9) |
                (bits.direction << 8) | (bits.size << 6) | (bits.rotmode << 5) |
                (bits.padding << 3) | (bits.datareg));
        }
        // Mem
        else
        {
            MemRepr bits;

            bits.direction = dirBit;

            bits.regtype = utils::to_val(instr.operands[0].operandType);
            bits.regmode =
                utils::to_val(instr.operands[0].specialAddressingMode);

            opcode.val = std::uint16_t(
                (bits.header << 9) | (bits.direction << 8) |
                (bits.padding << 6) | (bits.regtype << 3) | (bits.regmode));
        }
    }
} // namespace momiji::enc
