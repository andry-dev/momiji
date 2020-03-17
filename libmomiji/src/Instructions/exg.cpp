#include "exg.h"

#include "./Utils.h"

#include <asl/detect_features>
#include <asl/types>

namespace momiji::instr
{
#ifdef ASL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif
    momiji::System exg(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc = sys.cpu.programCounter;

        std::int32_t* srcreg = nullptr;
        const auto srcval    = utils::to_val(instr.addressingMode[0]);

        switch (instr.operandType[0])
        {
        case OperandType::DataRegister:
            srcreg = sys.cpu.dataRegisters[srcval].ptr();
            break;

        case OperandType::AddressRegister:
            srcreg = sys.cpu.addressRegisters[srcval].ptr();
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
            dstreg = asl::saccess(sys.cpu.dataRegisters, dstval).ptr();
            break;

        case OperandType::AddressRegister:
            dstreg = asl::saccess(sys.cpu.addressRegisters, dstval).ptr();
            break;

        default:
            // Nothing
            break;
        }

        std::swap(*srcreg, *dstreg);

        pc += 2;
        pc += std::uint32_t(utils::isImmediate(instr, 0));
        pc += std::uint32_t(utils::isImmediate(instr, 1));

        return sys;
    }
#ifdef ASL_CLANG
#pragma clang diagnostic pop
#endif
} // namespace momiji::instr
