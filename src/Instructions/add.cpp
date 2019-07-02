#include "add.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System add(momiji::System& sys, const InstructionData& data)
    {
        std::int32_t* srcreg = nullptr;

        const std::uint8_t srcnum = utils::to_val(data.mod1);

        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcreg = &sys.cpu.dataRegisters[srcnum].value;
            break;

        case OperandType::AddressRegister:
            srcreg = &sys.cpu.addressRegisters[srcnum].value;
            break;
        }

        std::int32_t* dstreg = nullptr;
        const std::uint8_t dstnum = utils::to_val(data.mod2);

        switch (data.op2)
        {
        case OperandType::DataRegister:
            dstreg = &sys.cpu.dataRegisters[dstnum].value;
            break;

        case OperandType::AddressRegister:
            dstreg = &sys.cpu.addressRegisters[dstnum].value;
            break;
        }

        std::int32_t tmp = 0;

        switch (data.size)
        {
        case 1:
            tmp = (*srcreg) & 0x000000FF;
            *dstreg = (*dstreg & 0xFFFFFF00) | ((*dstreg + tmp) & 0x000000FF);
            break;

        case 2:
            tmp = (*srcreg) & 0x0000FFFF;
            std::cout << tmp << '\n';
            *dstreg = (*dstreg & 0xFFFF0000) | ((*dstreg + tmp) & 0x0000FFFF);
            break;

        case 4:
            *dstreg = *dstreg + *srcreg;
            break;
        }

        return sys;
    }

    momiji::System adda(momiji::System& sys, const InstructionData& data)
    {
        std::int32_t* srcreg = nullptr;

        const std::uint8_t srcnum = utils::to_val(data.mod1);

        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcreg = &sys.cpu.dataRegisters[srcnum].value;
            break;

        case OperandType::AddressRegister:
            srcreg = &sys.cpu.addressRegisters[srcnum].value;
            break;
        }


        const std::uint8_t dstnum = utils::to_val(data.mod2);
        std::int32_t* dstreg = &sys.cpu.addressRegisters[dstnum].value;

        std::int32_t tmp = 0;

        switch (data.size)
        {
        case 2:
            tmp = utils::sign_extend<std::int16_t>(*srcreg);
            *dstreg = *dstreg + tmp;
            break;

        case 4:
            *dstreg = *dstreg + tmp;
            break;
        }

        return sys;
    }

    momiji::System addi(momiji::System& sys, const InstructionData& data)
    {
        auto pc = sys.cpu.programCounter.address;

        auto memview = momiji::make_memory_view(sys);

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
            srcval = memview[pc + 1] & 0x000000FF;
            *reg = (*reg & 0xFFFF'FF00) | ((*reg + srcval) & 0x0000'00FF);
            break;

        case 2:
            srcval = memview[pc + 1];
            *reg = (*reg & 0xFFFF'0000) | ((*reg + srcval) & 0x0000'FFFF);
            break;

        case 4:
            srcval = (memview[pc + 1] << 16) | memview[pc + 2];
            *reg = *reg + srcval;
            break;
        }

        return sys;
    }
}
