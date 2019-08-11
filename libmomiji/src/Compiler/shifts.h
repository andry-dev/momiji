#pragma once

#include <momiji/Compiler.h>

#include "../Instructions/Representations.h"
#include "./Utils.h"

#include <momiji/Utils.h>

namespace momiji::enc
{
    template <typename RegRepr, typename MemRepr>
    void any_shift(const ParsedInstruction& instr,
                   const momiji::LabelInfo& labels,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& /*additionalData*/,
                   std::uint8_t dirBit)
    {
        // Reg
        if (discriminateShifts(instr))
        {
            RegRepr bits;

            bits.direction = dirBit;
            bits.rotation  = 0;

            // clang-format off
            std::visit(asl::overloaded{
                [&] (const auto&) { },

                [&] (const operands::DataRegister& reg) {
                    bits.rotation = reg.reg;
                    bits.rotmode = 1;
                },
                
                [&] (const operands::Immediate& immediate) {
                    bits.rotation = momiji::resolveAST(*immediate.value, labels);
                    bits.rotmode = 0;
                }
            }, instr.operands[0]);
            // clang-format on

            bits.datareg = momiji::extractRegister(instr.operands[1]);
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

            bits.regtype = getCorrectOpType(instr.operands[0]);
            bits.regmode = getCorrectOpMode(instr.operands[0]);

            opcode.val = std::uint16_t(
                (bits.header << 9) | (bits.direction << 8) |
                (bits.padding << 6) | (bits.regtype << 3) | (bits.regmode));
        }
    }
} // namespace momiji::enc
