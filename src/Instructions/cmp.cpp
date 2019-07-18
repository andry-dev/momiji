#include "cmp.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System cmp(momiji::System& sys, const InstructionData& instr)
    {
        std::int32_t srcreg = 0;

        const std::uint8_t srcval = utils::to_val(instr.mod1);

        switch (instr.op1)
        {
        case OperandType::DataRegister:
            srcreg = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcreg = sys.cpu.addressRegisters[srcval].value;
            break;
        }

        // ""Destination"" register is a Data Register
        // The real modification happens in the status register

        const std::int8_t dstval = utils::to_val(instr.mod2);
        std::int32_t dstreg      = sys.cpu.dataRegisters[dstval].value;

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

        return sys;
    }

    momiji::System cmpa(momiji::System& sys, const InstructionData& instr)
    {
        std::int32_t srcreg = 0;

        const std::uint8_t srcval = utils::to_val(instr.mod1);

        switch (instr.op1)
        {
        case OperandType::DataRegister:
            srcreg = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcreg = sys.cpu.addressRegisters[srcval].value;
            break;
        }

        // ""Destination"" register is an Address Register
        // The real modification happens in the status register

        const std::int8_t dstval = utils::to_val(instr.mod2);
        std::int32_t dstreg      = sys.cpu.addressRegisters[dstval].value;

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

        return sys;
    }

    momiji::System cmpi(momiji::System& sys, const InstructionData& instr)
    {
        auto pc = sys.cpu.programCounter.address;

        auto memview = momiji::make_memory_view(sys);

        const std::int8_t dstval = utils::to_val(instr.mod2);
        std::int32_t dstreg      = sys.cpu.addressRegisters[dstval].value;

        switch (instr.op2)
        {
        case OperandType::DataRegister:
            dstreg = sys.cpu.dataRegisters[dstval].value;
            break;

        case OperandType::AddressRegister:
            dstreg = sys.cpu.addressRegisters[dstval].value;
            break;
        }

        std::int32_t srcval = 0;

        switch (instr.size)
        {
        case 1:
            srcval = memview.read8(pc + 2);
            srcval = utils::sign_extend<std::int8_t>(srcval);
            dstreg = utils::sign_extend<std::int8_t>(dstreg);
            break;

        case 2:
            srcval = memview.read16(pc + 2);
            srcval = utils::sign_extend<std::int16_t>(srcval);
            dstreg = utils::sign_extend<std::int16_t>(dstreg);
            break;

        case 4:
            srcval = memview.read32(pc + 2);
            break;
        }

        const std::int32_t res = dstreg - srcval;

        sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
        sys.cpu.statusRegister.zero     = (res == 0) ? 1 : 0;
        sys.cpu.statusRegister.overflow =
            utils::sub_overflow(dstreg, srcval) ? 1 : 0;

        return sys;
    }
} // namespace momiji::instr
