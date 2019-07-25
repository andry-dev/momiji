#include "jmp.h"

#include "./Utils.h"

namespace momiji::instr
{
    static std::uint32_t handleAddressResolution(momiji::System& sys,
                                                 const InstructionData& instr)
    {
        const auto& pc             = sys.cpu.programCounter.address;
        const std::uint32_t regnum = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        // (a*)
        case OperandType::Address:
        {
            return sys.cpu.addressRegisters[regnum].value;
        }
        break;

        // offset(a*)
        case OperandType::AddressOffset:
        {
            const std::uint32_t displacement =
                utils::readImmediateFromPC(sys.mem, pc, 2);

            return sys.cpu.addressRegisters[regnum].value + displacement;
        }
        break;

        // (offset, a*, **)
        case OperandType::AddressIndex:
        {
            const std::uint32_t immData =
                utils::readImmediateFromPC(sys.mem, pc, 2);

            const std::uint8_t newreg       = (immData & 0xF000) >> 12;
            const std::uint8_t displacement = (immData & 0x00FF);

            std::uint8_t index = 0;

            if (newreg < 8)
            {
                index = sys.cpu.dataRegisters[newreg].value;
            }
            else
            {
                index = sys.cpu.addressRegisters[newreg - 8].value;
            }

            return sys.cpu.addressRegisters[regnum].value + index +
                   displacement;
        }
        break;

        case OperandType::Immediate:
            switch (instr.addressingMode[0])
            {
            case SpecialAddressingMode::AbsoluteShort:
                return utils::readImmediateFromPC(sys.mem, pc, 2);
                break;

            case SpecialAddressingMode::AbsoluteLong:
            case SpecialAddressingMode::Immediate:
                return utils::readImmediateFromPC(sys.mem, pc, 4);
                break;

            default:
                // None
                break;
            }
            break;

        default:
            // None
            break;
        }

        return sys.cpu.programCounter.address;
    }

    momiji::System jmp(momiji::System& sys, const InstructionData& data)
    {
        sys.cpu.programCounter.address = handleAddressResolution(sys, data);

        return sys;
    }

    momiji::System jsr(momiji::System& sys, const InstructionData& data)
    {
        auto& sp = sys.cpu.addressRegisters[7].value;
        auto& pc = sys.cpu.programCounter.address;

        sp -= 4;

        std::uint32_t* retaddr =
            reinterpret_cast<std::uint32_t*>(sys.mem.data() + sp);

        *retaddr = pc;

        pc = handleAddressResolution(sys, data);

        return sys;
    }
} // namespace momiji::instr
