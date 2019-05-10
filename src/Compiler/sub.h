#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void sub(const momiji::Instruction& instr,
             MemoryType&,
             OpcodeDescription& opcode,
             AdditionalData&)
    {
        repr::Sub bits;

        if (instr.operands[0].operandType == OperandType::DataRegister)
        {
            bits.datareg = (instr.operands[0].value & 0b111);
            bits.direction = 1;
            bits.othmode = utils::to_val(instr.operands[1].operandType) & 0b111;
            bits.oth = instr.operands[1].value & 0b111;
        }
        else
        {
            bits.datareg = (instr.operands[1].value & 0b111);
            bits.direction = 0;
            bits.othmode = utils::to_val(instr.operands[0].operandType) & 0b111;
            bits.oth = instr.operands[0].value & 0b111;
        }

        std::uint8_t size = utils::to_val(instr.dataType);
        bits.size = size & 0b11;

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.direction << 8)
                    | (bits.size << 6)
                    | (bits.othmode << 3)
                    | (bits.oth);
    }

    void suba(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData&)
    {
        repr::SubA bits;

        bits.addreg = instr.operands[1].value & 0b111;
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

        bits.srcmode = utils::to_val(instr.operands[0].operandType) & 0b111;
        bits.src = instr.operands[0].value & 0b111;

        opcode.val =  (bits.header << 12)
                    | (bits.addreg << 9)
                    | (bits.size << 8)
                    | (bits.padding << 6)
                    | (bits.srcmode << 3)
                    | (bits.src);
    }

    void subi(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;

        repr::SubI bits;

        bits.size = size & 0b11;

        additionalData.cnt = tobyte[size];
        additionalData.val = instr.operands[0].value;

        bits.dstmode = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dst = instr.operands[1].value & 0b111;

        opcode.val =  (bits.header << 8)
                    | (bits.size << 6)
                    | (bits.dstmode << 3)
                    | (bits.dst);
    }
}
