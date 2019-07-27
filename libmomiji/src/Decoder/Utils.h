#pragma once

#include <momiji/Decoder.h>

#include <momiji/Utils.h>

namespace momiji
{
    inline void assignNormalSize(DecodedInstruction& instr, std::uint8_t size)
    {
        switch (size)
        {
        case 0b00:
            instr.data.size = 1;
            break;

        case 0b01:
            instr.data.size = 2;
            break;

        case 0b10:
            instr.data.size = 4;
            break;
        }
    }

    inline std::string dataTypeToString(int size)
    {
        switch (size)
        {
        case 1:
            return ".b";

        case 2:
            return ".w";

        case 4:
            return ".l";
        }

        return "";
    }

    inline std::string opToString(const InstructionData& instr,
                                  int opNum,
                                  ExecutableMemoryView mem,
                                  std::uint64_t idx)
    {
        const std::int32_t optype = utils::to_val(instr.operandType[opNum]);
        const std::int32_t opmode = utils::to_val(instr.addressingMode[opNum]);

        idx += 2;

        switch (instr.operandType[opNum])
        {
        case OperandType::DataRegister:
            return "d" + std::to_string(opmode);

        case OperandType::AddressRegister:
            return "a" + std::to_string(opmode);

        case OperandType::AddressPre:
            return "-(a" + std::to_string(opmode) + ")";

        case OperandType::AddressPost:
            return "(a" + std::to_string(opmode) + ")+";

        case OperandType::Address:
            return "(a" + std::to_string(opmode) + ")";
            break;

        case OperandType::AddressOffset:
        {
            std::int16_t offset =
                mem.read16(idx + utils::resolveOp1Size(instr, opNum));

            return std::to_string(offset) + "(a" + std::to_string(opmode) + ")";
        }
        break;

        case OperandType::AddressIndex:
        {
            std::int16_t imm =
                mem.read16(idx + utils::resolveOp1Size(instr, opNum));

            std::int8_t reg    = (imm & 0xF000) >> 12;
            std::int8_t offset = (imm & 0x00FF);

            std::string ret = "(" + std::to_string(offset) + ", a" +
                              std::to_string(opmode) + ", ";

            if (offset < 8)
            {
                ret += "d" + std::to_string(offset) + ")";
            }
            else
            {
                ret += "a" + std::to_string(offset - 8) + ")";
            }

            return ret;
        }
        break;

        case OperandType::Immediate:
            switch (instr.addressingMode[opNum])
            {
            case SpecialAddressingMode::Immediate:
            {
                switch (instr.size)
                {
                case 1:
                    [[fallthrough]];
                case 2:
                {
                    std::int16_t val =
                        mem.read16(idx + utils::resolveOp1Size(instr, opNum));
                    return "#" + std::to_string(val);
                }

                case 4:
                {
                    std::int32_t val =
                        mem.read32(idx + utils::resolveOp1Size(instr, opNum));
                    return "#" + std::to_string(val);
                }
                }
            }
            break;

            case SpecialAddressingMode::AbsoluteShort:
            {
                std::int16_t addr =
                    mem.read16(idx + utils::resolveOp1Size(instr, opNum));

                return std::to_string(addr);
            }
            break;

            case SpecialAddressingMode::AbsoluteLong:
            {
                std::int32_t addr =
                    mem.read32(idx + utils::resolveOp1Size(instr, opNum));

                return std::to_string(addr);
            }
            break;

            default:
                return "???";
            }
            break;
        }

        return "???";
    }

    inline std::string commonStringConverter(const InstructionData& instr,
                                             ExecutableMemoryView mem,
                                             std::uint64_t idx)
    {
        return opToString(instr, 0, mem, idx) + ", " +
               opToString(instr, 1, mem, idx);
    }
} // namespace momiji
