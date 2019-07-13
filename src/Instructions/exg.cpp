#include "exg.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System exg(momiji::System& sys, const InstructionData& instr)
    {
        std::int32_t* srcreg = nullptr;
        const std::int8_t srcval = utils::to_val(instr.mod1);

        switch (instr.op1)
        {
        case OperandType::DataRegister:
            srcreg = &sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcreg = &sys.cpu.addressRegisters[srcval].value;
            break;
        }

        std::int32_t* dstreg = nullptr;
        const std::int8_t dstval = utils::to_val(instr.mod2);

        switch (instr.op2)
        {
        case OperandType::DataRegister:
            dstreg = &sys.cpu.dataRegisters[dstval].value;
            break;

        case OperandType::AddressRegister:
            dstreg = &sys.cpu.addressRegisters[dstval].value;
            break;
        }

        std::swap(*srcreg, *dstreg);

        return sys;
    }
} // namespace momiji::instr
