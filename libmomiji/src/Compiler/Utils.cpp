#include "Utils.h"

#include <momiji/Memory.h>
#include <momiji/Utils.h>

namespace momiji
{
    std::uint8_t getCorrectOpMode(const momiji::Instruction& instr,
                                  std::int8_t opNum)
    {
        const auto& op = asl::saccess(instr.operands, opNum);
        switch (op.operandType)
        {
        case OperandType::DataRegister:
            [[fallthrough]];
        case OperandType::AddressRegister:
            [[fallthrough]];
        case OperandType::Address:
            [[fallthrough]];
        case OperandType::AddressPost:
            [[fallthrough]];
        case OperandType::AddressPre:
            [[fallthrough]];
        case OperandType::AddressIndex:
            [[fallthrough]];
        case OperandType::AddressOffset:
            return op.value & 0b111;

        default:
            return utils::to_val(op.specialAddressingMode);
        }
    }

    void handleAdditionalData(const momiji::Instruction& instr,
                              std::array<AdditionalData, 2>& additionalData)
    {
        Expects(instr.operands.size() <= 2,
                "How could you even get more than 2 operands here?");

        auto size = utils::to_val(instr.dataType);

        for (std::int64_t i = 0; i < asl::ssize(instr.operands); ++i)
        {
            const auto& op = asl::saccess(instr.operands, i);
            auto& data     = asl::saccess(additionalData, i);

            switch (op.operandType)
            {
            // offset(a*)
            case OperandType::AddressIndex:
                [[fallthrough]];
            // (offset, a*, X*)
            case OperandType::AddressOffset:
                data.cnt = 2;
                data.val = (std::uint32_t(op.value) & 0xFFFF'0000) >> 16;
                break;

            case OperandType::Immediate:
                switch (op.specialAddressingMode)
                {
                // #1234
                case SpecialAddressingMode::Immediate:
                    data.cnt = tobyte[size];
                    data.val = std::uint32_t(op.value);
                    break;

                // *.w 1234
                case SpecialAddressingMode::AbsoluteShort:
                    data.cnt = 2;
                    data.val = std::uint32_t(op.value);
                    break;

                // *.l 1234
                case SpecialAddressingMode::AbsoluteLong:
                    data.cnt = 4;
                    data.val = std::uint32_t(op.value);
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    [[fallthrough]];
                case SpecialAddressingMode::ProgramCounterIndex:
                    // TODO(andry): Implement PC addressing
                    break;
                }
                break;

            default:
                break;
            }
        }
    }

    bool discriminateShifts(const Instruction& instr)
    {
        if ((instr.operands[0].operandType != OperandType::DataRegister) &&
            ((instr.operands[0].operandType != OperandType::Immediate) &&
             (instr.operands[0].specialAddressingMode !=
              SpecialAddressingMode::Immediate)))
        {
            return false;
        }

        return true;
    }
} // namespace momiji
