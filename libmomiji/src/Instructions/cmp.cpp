#include "cmp.h"

#include "./Utils.h"
#include <System.h>

namespace momiji::instr
{
    momiji::System cmp(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc = sys.cpu.programCounter;

        std::int32_t srcreg = 0;

        const std::uint8_t srcval = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        case OperandType::DataRegister:
            srcreg = sys.cpu.dataRegisters[srcval].raw();
            break;

        case OperandType::AddressRegister:
            srcreg = sys.cpu.addressRegisters[srcval].raw();
            break;

        default:
            break;
        }

        // ""Destination"" register is a Data Register
        // The real modification happens in the status register

        const auto dstval = std::int8_t(utils::to_val(instr.addressingMode[1]));

        auto dstreg = asl::saccess(sys.cpu.dataRegisters, dstval).raw();

        switch (instr.size)
        {
        case 1:
            srcreg = utils::sign_extend<std::int8_t>(srcreg);
            dstreg = utils::sign_extend<std::int8_t>(dstreg);
            break;

        case 2:
            srcreg = utils::sign_extend<std::int16_t>(srcreg);
            dstreg = utils::sign_extend<std::int16_t>(dstreg);
            break;
        }

        const std::int32_t res = dstreg - srcreg;

        sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
        sys.cpu.statusRegister.zero     = (res == 0) ? 1 : 0;
        sys.cpu.statusRegister.overflow =
            utils::sub_overflow(dstreg, srcreg) ? 1 : 0;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(instr, 0));

        return sys;
    }

    momiji::System cmpa(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc            = sys.cpu.programCounter;
        std::int32_t srcreg = 0;

        const std::uint8_t srcval = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        case OperandType::DataRegister:
            srcreg = sys.cpu.dataRegisters[srcval].raw();
            break;

        case OperandType::AddressRegister:
            srcreg = sys.cpu.addressRegisters[srcval].raw();
            break;

        default:
            break;
        }

        // ""Destination"" register is an Address Register
        // The real modification happens in the status register

        const auto dstval = std::int8_t(utils::to_val(instr.addressingMode[1]));

        std::int32_t dstreg =
            asl::saccess(sys.cpu.addressRegisters, dstval).raw();

        switch (instr.size)
        {
        case 1:
            srcreg = utils::sign_extend<std::int8_t>(srcreg);
            dstreg = utils::sign_extend<std::int8_t>(dstreg);
            break;

        case 2:
            srcreg = utils::sign_extend<std::int16_t>(srcreg);
            dstreg = utils::sign_extend<std::int16_t>(dstreg);
            break;
        }

        const std::int32_t res = dstreg - srcreg;

        sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
        sys.cpu.statusRegister.zero     = (res == 0) ? 1 : 0;
        sys.cpu.statusRegister.overflow =
            utils::sub_overflow(dstreg, srcreg) ? 1 : 0;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(instr, 0));
        pc += std::uint8_t(utils::isImmediate(instr, 1));

        return sys;
    }

    momiji::System cmpi(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc = sys.cpu.programCounter;

        auto memview = momiji::make_memory_view(sys);

        const auto dstval = std::int8_t(utils::to_val(instr.addressingMode[1]));
        auto dstreg       = asl::saccess(sys.cpu.dataRegisters, dstval).raw();

        switch (instr.operandType[1])
        {
        case OperandType::DataRegister:
            dstreg = asl::saccess(sys.cpu.dataRegisters, dstval).raw();
            break;

        case OperandType::AddressRegister:
            dstreg = asl::saccess(sys.cpu.addressRegisters, dstval).raw();
            break;

        default:
            break;
        }

        std::int32_t srcval = 0;

        const auto nextloc = pc + 2;
        switch (instr.size)
        {
        case 1:
            srcval = *memview.read8(nextloc.raw());
            srcval = utils::sign_extend<std::int8_t>(srcval);
            dstreg = utils::sign_extend<std::int8_t>(dstreg);
            break;

        case 2:
            srcval = *memview.read16(nextloc.raw());
            srcval = utils::sign_extend<std::int16_t>(srcval);
            dstreg = utils::sign_extend<std::int16_t>(dstreg);
            break;

        case 4:
            srcval = std::int32_t(*memview.read32(nextloc.raw()));
            break;
        }

        const std::int32_t res = dstreg - srcval;

        sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
        sys.cpu.statusRegister.zero     = (res == 0) ? 1 : 0;
        sys.cpu.statusRegister.overflow =
            utils::sub_overflow(dstreg, srcval) ? 1 : 0;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(instr, 0));
        pc += std::uint8_t(utils::isImmediate(instr, 1));

        return sys;
    }
} // namespace momiji::instr
