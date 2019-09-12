#include "xor.h"

#include <momiji/Utils.h>

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

namespace momiji::enc
{
    void xor_instr(const momiji::ParsedInstruction& instr,
                   const momiji::LabelInfo& /*labels*/,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Xor bits;

        // eor.* d*, *
        bits.datareg = std::uint16_t(
            std::get<operands::DataRegister>(instr.operands[0]).reg);

        bits.othtype = getCorrectOpType(instr.operands[1]);
        bits.othmode = getCorrectOpMode(instr.operands[1]);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 8) | (bits.size << 6) |
                                   (bits.othtype << 3) | (bits.othmode));
    }

    void xori(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::XorI bits;

        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;
        bits.size               = size & 0b111;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val =
            std::uint32_t(extractASTValue(instr.operands[0], labels));

        bits.dsttype = getCorrectOpType(instr.operands[1]);
        bits.dstmode = getCorrectOpMode(instr.operands[1]);

        opcode.val = std::uint16_t((bits.header << 8) | (bits.size << 6) |
                                   (bits.dsttype << 3) | (bits.dstmode));
    }
} // namespace momiji::enc
