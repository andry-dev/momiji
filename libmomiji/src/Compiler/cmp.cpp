#include "cmp.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void cmp(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Cmp bits;

        bits.datareg = std::get<operands::DataRegister>(instr.operands[1]).reg;
        bits.size    = utils::to_val(instr.dataType) & 0b11;
        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 8) | (bits.size << 6) |
                                   (bits.srctype << 3) | (bits.srcmode));
    }

    void cmpa(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::CmpA bits;

        bits.addreg =
            std::get<operands::AddressRegister>(instr.operands[1]).reg;

        switch (instr.dataType)
        {
        case DataType::Byte:
            opcode.val = repr::Illegal {}.value;
            return;

        case DataType::Word:
            bits.size = 0;
            break;

        case DataType::Long:
            bits.size = 1;
            break;
        }

        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.addreg << 9) |
                                   (bits.size << 8) | (bits.padding << 6) |
                                   (bits.srctype << 3) | (bits.srcmode));
    }

    void cmpi(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::CmpI bits;

        const std::uint8_t size = utils::to_val(instr.dataType);

        bits.size = size & 0b11;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val = extractASTValue(instr.operands[0], labels);

        bits.dsttype = getCorrectOpType(instr.operands[1]);
        bits.dstmode = getCorrectOpMode(instr.operands[1]);

        opcode.val = std::uint16_t((bits.header << 8) | (bits.size << 6) |
                                   (bits.dsttype << 3) | (bits.dstmode));
    }
} // namespace momiji::enc
