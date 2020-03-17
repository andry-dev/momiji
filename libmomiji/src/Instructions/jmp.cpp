#include "jmp.h"

#include "./Utils.h"

namespace momiji::instr
{
    static std::uint32_t handleAddressResolution(momiji::System& sys,
                                                 const InstructionData& instr)
    {
        const auto& pc             = sys.cpu.programCounter;
        const std::uint32_t regnum = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        // (a*)
        case OperandType::Address: {
            return std::uint32_t(sys.cpu.addressRegisters[regnum].raw());
        }

        // offset(a*)
        case OperandType::AddressOffset: {
            const std::int32_t displacement =
                utils::readImmediateFromPC(sys.mem, pc, 2);

            return std::uint32_t(sys.cpu.addressRegisters[regnum].raw() +
                                 displacement);
        }

        // (offset, a*, **)
        case OperandType::AddressIndex: {
            const std::int32_t immData =
                utils::readImmediateFromPC(sys.mem, pc, 2);

            const auto newreg       = std::int8_t((immData & 0xF000) >> 12);
            const auto displacement = std::int8_t(immData & 0x00FF);

            std::int32_t index = 0;

            if (newreg < 8)
            {
                index = asl::saccess(sys.cpu.dataRegisters, newreg).raw();
            }
            else
            {
                index =
                    asl::saccess(sys.cpu.addressRegisters, newreg - 8).raw();
            }

            return std::uint32_t(sys.cpu.addressRegisters[regnum].raw() +
                                 index + displacement);
        }

        case OperandType::Immediate:
            switch (instr.addressingMode[0])
            {
            case SpecialAddressingMode::AbsoluteShort:
                return std::uint32_t(
                    utils::readImmediateFromPC(sys.mem, pc, 2));

            case SpecialAddressingMode::AbsoluteLong:
            case SpecialAddressingMode::Immediate:
                return std::uint32_t(
                    utils::readImmediateFromPC(sys.mem, pc, 4));

            default:
                // None
                break;
            }
            break;

        default:
            // None
            break;
        }

        return sys.cpu.programCounter.raw();
    }

    momiji::System jmp(momiji::System& sys, const InstructionData& data)
    {
        sys.cpu.programCounter = handleAddressResolution(sys, data);

        return sys;
    }

    momiji::System jsr(momiji::System& sys, const InstructionData& data)
    {
        auto& sp = sys.cpu.addressRegisters[7];
        auto& pc = sys.cpu.programCounter;

        sp -= 4;

        auto retaddr =
            reinterpret_cast<std::uint32_t*>(sys.mem.data() + sp.raw());

        *retaddr = pc.raw();

        pc = handleAddressResolution(sys, data);

        return sys;
    }
} // namespace momiji::instr
