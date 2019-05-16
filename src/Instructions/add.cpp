#include "add.h"

#include <iostream>

namespace momiji
{
    namespace instr
    {
        momiji::System add(momiji::System sys, const InstructionData& data)
        {
            return sys;
        }

        momiji::System addi(momiji::System sys, const InstructionData& data)
        {
            auto* pc = sys.cpu.programCounter.address;

            std::int32_t dstval = utils::to_val(data.mod2);
            std::int32_t* reg = nullptr;
            switch (data.op2)
            {
            case OperandType::DataRegister:
                reg = &sys.cpu.dataRegisters[dstval].value;
                break;

            case OperandType::AddressRegister:
                reg = &sys.cpu.addressRegisters[dstval].value;
                break;
            }

            std::int32_t srcval = 0;

            switch (data.size)
            {
            case 1:
                srcval = *(pc + 1) & 0x000000FF;
                *reg = (*reg & 0xFFFF'FF00) | ((*reg + srcval) & 0x0000'00FF);
                break;
            case 2:
                srcval = *(pc + 1);
                *reg = (*reg & 0xFFFF'0000) | ((*reg + srcval) & 0x0000'FFFF);
                break;

            case 4:
                srcval = (*(pc + 1) << 16) | *(pc + 2);
                *reg = *reg + srcval;
                break;
            }

            return sys;
        }
    }
}
