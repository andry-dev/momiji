#include "exg.h"

#include "./Utils.h"

namespace momiji::instr
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
    momiji::System exg(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t* srcreg     = nullptr;
        const std::int8_t srcval = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        case OperandType::DataRegister:
            srcreg = &sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcreg = &sys.cpu.addressRegisters[srcval].value;
            break;

        default:
            // Nothing
            break;
        }

        std::int32_t* dstreg = nullptr;
        const auto dstval = std::int8_t(utils::to_val(instr.addressingMode[1]));

        switch (instr.operandType[1])
        {
        case OperandType::DataRegister:
            dstreg = &sys.cpu.dataRegisters[dstval].value;
            break;

        case OperandType::AddressRegister:
            dstreg = &sys.cpu.addressRegisters[dstval].value;
            break;

        default:
            // Nothing
            break;
        }

        std::swap(*srcreg, *dstreg);

        pc += 2;
        pc += utils::isImmediate(instr, 0);
        pc += utils::isImmediate(instr, 1);

        return sys;
    }
#pragma clang diagnostic pop
} // namespace momiji::instr
