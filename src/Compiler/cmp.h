#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void cmp(const momiji::Instruction& instr,
             MemoryType&,
             OpcodeDescription& opcode,
             AdditionalData& additionalData)
    {
        repr::Cmp bits;

        bits.datareg = instr.operands[1].value & 0b111;
        bits.size = utils::to_val(instr.dataType) & 0b11;
        bits.srcmode = utils::to_val(instr.operands[0].operandType) & 0b111;
        bits.src = instr.operands[0].value & 0b111;

        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.padding << 8)
                    | (bits.size << 6)
                    | (bits.srcmode << 3)
                    | (bits.src);
    }

    void cmpa(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::CmpA bits;

        bits.addreg = instr.operands[1].value & 0b111;

        switch (instr.dataType)
        {
        case DataType::Byte:
            opcode.val = repr::Illegal{}.value;
            return;
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

    void cmpi(const momiji::Instruction& instr,
              MemoryType&,
              OpcodeDescription& opcode,
              AdditionalData& additionalData)
    {
        repr::CmpI bits;

        const std::uint8_t size = utils::to_val(instr.dataType);

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
