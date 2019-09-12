#include "and.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void and_instr(const momiji::ParsedInstruction& instr,
                   const momiji::LabelInfo& labels,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData)
    {
        repr::And bits;

        // and.* *, d*
        if (matchOperand<operands::DataRegister>(instr.operands[1]))
        {
            bits.datareg = std::uint16_t(
                std::get<operands::DataRegister>(instr.operands[1]).reg);
            bits.direction = 0;
        }
        // and.* d*, *
        else
        {
            bits.datareg = std::uint16_t(
                std::get<operands::DataRegister>(instr.operands[0]).reg);
            bits.direction = 1;
        }

        bits.othtype = getCorrectOpType(instr.operands[bits.direction]);
        bits.othmode = getCorrectOpMode(instr.operands[bits.direction]);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.direction << 8) | (bits.size << 6) |
                                   (bits.othtype << 3) | (bits.othmode));
    }

    void andi(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::AndI bits;

        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;
        bits.size               = size & 0b111;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val =
            std::uint32_t(extractASTValue(instr.operands[0], labels));

        bits.dsttype = getCorrectOpType(instr.operands[1]);
        bits.dstmode = getCorrectOpMode(instr.operands[1]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 8) | (bits.size << 6) |
                                   (bits.dsttype << 3) | (bits.dstmode));
    }
} // namespace momiji::enc
