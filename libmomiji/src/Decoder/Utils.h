#pragma once

#include <momiji/Decoder.h>

#include <momiji/Utils.h>

namespace momiji
{
    inline void assignNormalSize(DecodedInstruction& instr, std::int8_t size)
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

    inline std::string dataTypeToString(std::int8_t size)
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
                                  std::int8_t opNum,
                                  ConstExecutableMemoryView mem,
                                  std::int64_t idx)
    {
        const std::int32_t opmode =
            utils::to_val(asl::saccess(instr.addressingMode, opNum));

        idx += 2;

        switch (asl::saccess(instr.operandType, opNum))
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

        case OperandType::AddressOffset:
        {
            auto offset = mem.read16(idx + utils::resolveOp1Size(instr, opNum));

            return std::to_string(*offset) + "(a" + std::to_string(opmode) +
                   ")";
        }

        case OperandType::AddressIndex:
        {
            auto imm = mem.read16(idx + utils::resolveOp1Size(instr, opNum));

            const auto reg    = std::uint8_t((*imm & 0xF000) >> 12);
            const auto offset = std::uint8_t((*imm & 0x00FF));

            std::string ret = "(" + std::to_string(offset) + ", a" +
                              std::to_string(opmode) + ", ";

            if (reg < 8)
            {
                ret += "d" + std::to_string(reg) + ")";
            }
            else
            {
                ret += "a" + std::to_string(reg - 8) + ")";
            }

            return ret;
        }

        case OperandType::Immediate:
            switch (asl::saccess(instr.addressingMode, opNum))
            {
            case SpecialAddressingMode::Immediate:
            {
                switch (instr.size)
                {
                case 1:
                    [[fallthrough]];
                case 2:
                {
                    auto val =
                        mem.read16(idx + utils::resolveOp1Size(instr, opNum));
                    return "#" + std::to_string(*val);
                }

                case 4:
                {
                    auto val =
                        mem.read32(idx + utils::resolveOp1Size(instr, opNum));
                    return "#" + std::to_string(*val);
                }
                }
            }
            break;

            case SpecialAddressingMode::AbsoluteShort:
            {
                auto addr =
                    mem.read16(idx + utils::resolveOp1Size(instr, opNum));

                return std::to_string(*addr);
            }

            case SpecialAddressingMode::AbsoluteLong:
            {
                auto addr =
                    mem.read32(idx + utils::resolveOp1Size(instr, opNum));

                return std::to_string(*addr);
            }

            default:
                return "???";
            }
            break;
        }

        return "???";
    }

    inline std::string commonStringConverter(const InstructionData& instr,
                                             ConstExecutableMemoryView mem,
                                             std::int64_t idx)
    {
        return opToString(instr, 0, mem, idx) + ", " +
               opToString(instr, 1, mem, idx);
    }
} // namespace momiji
