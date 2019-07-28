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
                const std::int32_t msb = (reg & mask);
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
        std::int32_t mask = 0b10000000'00000000'00000000'00000000;

        auto& pc = sys.cpu.programCounter.address;
        pc += 2;

        switch (instr.size)
        {
        case 1:
            mask = 0b00000000'00000000'00000000'10000000;
            break;

        case 2:
            mask = 0b00000000'00000000'10000000'00000000;
            break;
        }

        if (instr.operandType[1] == OperandType::Address)
        {
            // Memory shift
            std::int16_t* dst = utils::readOperandPtr16(sys, instr, 0);

            auto& dstref = *dst;
            ShiftType::compute(dstref, 1, mask);

            pc += utils::isImmediate(instr, 0);
        }
        else
        {
            // Reg shift
            const std::int8_t src     = utils::to_val(instr.addressingMode[0]);
            const std::int32_t dstreg = utils::to_val(instr.addressingMode[1]);

            auto& dst = sys.cpu.dataRegisters[dstreg].value;

            if (instr.operandType[0] == OperandType::Immediate)
            {
                ShiftType::compute(dst, src, mask);
            }
            else
            {
                ShiftType::compute(
                    dst, sys.cpu.dataRegisters[src].value % 64, mask);
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
