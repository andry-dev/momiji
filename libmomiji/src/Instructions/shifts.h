#pragma once

#include <momiji/System.h>

#include "./Utils.h"

namespace momiji::instr
{
    namespace details
    {
        struct ArithShiftRight
        {
            template <typename T>
            static void compute(T& reg, std::int8_t count, std::int32_t mask)
            {
                const auto msb = (reg & mask);
                reg >>= count;
                reg |= msb;
            }
        };

        struct ArithShiftLeft
        {
            template <typename T>
            static void
            compute(T& reg, std::int8_t count, std::int32_t /*mask*/)
            {
                reg <<= count;
            }
        };

        struct LogicalShiftRight
        {
            template <typename T>
            static void
            compute(T& reg, std::int8_t count, std::int32_t /*mask*/)
            {
                reg >>= count;
            }
        };

        struct LogicalShiftLeft
        {
            template <typename T>
            static void
            compute(T& reg, std::int8_t count, std::int32_t /*mask*/)
            {
                reg <<= count;
            }
        };
    } // namespace details

    template <typename ShiftType>
    momiji::System shift(momiji::System& sys, const InstructionData& instr)
    {
        const auto mask = [&]() -> std::int32_t {
            switch (instr.size)
            {
            case 1:
                return std::int32_t(0b00000000'00000000'00000000'10000000);

            case 2:
                return std::int32_t(0b00000000'00000000'10000000'00000000);

            default:
                return std::int32_t(0b10000000'00000000'00000000'00000000);
            }
        }();

        auto& pc = sys.cpu.programCounter;
        pc += 2;

        if (instr.operandType[1] == OperandType::Address)
        {
            // Memory shift
            auto dst = utils::readOperandPtr<std::int16_t>(sys, instr, 0);

            auto& dstref = *dst;
            ShiftType::compute(dstref, 1, mask);

            pc += std::uint8_t(utils::isImmediate(instr, 0));
        }
        else
        {
            // Reg shift
            const auto src =
                std::int8_t(utils::to_val(instr.addressingMode[0]));

            const auto dstreg =
                std::int32_t(utils::to_val(instr.addressingMode[1]));

            auto& dst = asl::saccess(sys.cpu.dataRegisters, dstreg);

            if (instr.operandType[0] == OperandType::Immediate)
            {
                ShiftType::compute(dst, src, mask);
            }
            else
            {
                const auto reg = asl::saccess(sys.cpu.dataRegisters, src);
                ShiftType::compute(dst, reg.raw() % 64, mask);
            }
        }

        return sys;
    }

    template System shift<details::ArithShiftLeft>(System&,
                                                   const InstructionData&);
    template System shift<details::ArithShiftRight>(System&,
                                                    const InstructionData&);
    template System shift<details::LogicalShiftLeft>(System&,
                                                     const InstructionData&);
    template System shift<details::LogicalShiftRight>(System&,
                                                      const InstructionData&);
} // namespace momiji::instr
