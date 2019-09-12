#include "sub.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void sub(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Sub bits;

        if (matchOperand<operands::DataRegister>(instr.operands[0]))
        {
            bits.datareg = std::uint16_t(
                std::get<operands::DataRegister>(instr.operands[0]).reg);
            bits.direction = 1;
        }
        else
        {
            bits.datareg = std::uint16_t(
                std::get<operands::DataRegister>(instr.operands[1]).reg);
            bits.direction = 0;
        }

        bits.othtype = getCorrectOpType(instr.operands[bits.direction]);
        bits.othmode = getCorrectOpMode(instr.operands[bits.direction]);

        std::uint8_t size = utils::to_val(instr.dataType);
        bits.size         = size & 0b11;

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.direction << 8) | (bits.size << 6) |
                                   (bits.othtype << 3) | (bits.othmode));
    }

    void suba(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::SubA bits;

        bits.addreg = std::uint16_t(
            std::get<operands::AddressRegister>(instr.operands[1]).reg);

        switch (instr.dataType)
        {
        case DataType::Byte:
            break;

        case DataType::Word:
            bits.size = 0;
            break;

        case DataType::Long:
            bits.size = 1;
            break;
        }

        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.addreg << 9) |
                                   (bits.size << 8) | (bits.padding << 6) |
                                   (bits.srctype << 3) | (bits.srcmode));
    }

    void subi(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        const std::uint8_t size = utils::to_val(instr.dataType);

        repr::SubI bits;

        bits.size = size & 0b11;

        bits.dsttype = getCorrectOpType(instr.operands[1]);
        bits.dstmode = getCorrectOpMode(instr.operands[1]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 8) | (bits.size << 6) |
                                   (bits.dsttype << 3) | (bits.dstmode));
    }
} // namespace momiji::enc
