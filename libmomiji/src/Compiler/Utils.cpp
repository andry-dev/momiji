#include "Utils.h"

#include <momiji/Memory.h>
#include <momiji/Utils.h>

namespace momiji
{
    // Transforms the instruction to the bits representing that addressing
    // mode OR a register
    std::uint8_t getCorrectOpMode(const momiji::Operand& op)
    {
        std::uint8_t val = 0;

        namespace ops = momiji::operands;

        // clang-format off
            std::visit(asl::overloaded{
                [&] (const auto& reg) {
                    val = reg.reg & 0b111;
                },

                [&] (const ops::Immediate&) {
                    val = utils::to_val(SpecialAddressingMode::Immediate);
                },

                [&] (const ops::ProgramCounterIndex&) {
                    val = utils::to_val(
                            SpecialAddressingMode::ProgramCounterIndex);
                },

                [&] (const ops::ProgramCounterOffset&) {
                    val = utils::to_val(
                            SpecialAddressingMode::ProgramCounterOffset);
                },

                [&] (const ops::AbsoluteShort&) {
                    val = utils::to_val(SpecialAddressingMode::AbsoluteShort);
                },

                [&] (const ops::AbsoluteLong&) {
                    val = utils::to_val(SpecialAddressingMode::AbsoluteLong);
                }
            }, op);
        // clang-format on

        return val & 0b111;
    }

    std::uint8_t getCorrectOpType(const momiji::Operand& op)
    {
        std::uint8_t val = 0;

        namespace ops = momiji::operands;

        // clang-format off
            std::visit(asl::overloaded{
                [&] (const ops::DataRegister&) {
                    val = utils::to_val(OperandType::DataRegister);
                },

                [&] (const ops::AddressRegister&) {
                    val = utils::to_val(OperandType::AddressRegister);
                },

                [&] (const ops::Address&) {
                    val = utils::to_val(OperandType::Address);
                },

                [&] (const ops::AddressPre&) {
                    val = utils::to_val(OperandType::AddressPre);
                },

                [&] (const ops::AddressPost&) {
                    val = utils::to_val(OperandType::AddressPost);
                },

                [&] (const ops::AddressOffset&) {
                    val = utils::to_val(OperandType::AddressOffset);
                },

                [&] (const ops::AddressIndex&) {
                    val = utils::to_val(OperandType::AddressIndex);
                },

                [&] (const ops::AbsoluteShort&) {
                    val = utils::to_val(OperandType::AbsoluteShort);
                },

                [&] (const ops::AbsoluteLong&) {
                    val = utils::to_val(OperandType::AbsoluteLong);
                },

                [&] (const ops::Immediate&) {
                    val = utils::to_val(OperandType::Immediate);
                },

                [&] (const ops::ProgramCounterOffset&) {
                    val = utils::to_val(OperandType::ProgramCounterOffset);
                },

                [&] (const ops::ProgramCounterIndex&) {
                    val = utils::to_val(OperandType::ProgramCounterIndex);
                },

            }, op);
        // clang-format on

        return val & 0b111;
    }

    void handleAdditionalData(const momiji::ParsedInstruction& instr,
                              const momiji::LabelInfo& labels,
                              std::array<AdditionalData, 2>& additionalData)
    {
        Expects(instr.operands.size() <= 2,
                "How could you even get more than 2 operands here?");

        auto size = utils::to_val(instr.dataType);

        for (std::int64_t i = 0; i < asl::ssize(instr.operands); ++i)
        {
            const auto& op = asl::saccess(instr.operands, i);
            auto& data     = asl::saccess(additionalData, i);

            namespace ops = momiji::operands;

            // clang-format off
                std::visit(asl::overloaded{
                    [&data] (const auto& reg) {
                    },

                    [&data, &labels] (const ops::AddressOffset& reg) {
                        Expects(reg.offset.get(), "Null pointer for address offset")

                        data.cnt = 2;
                        data.val = momiji::resolveAST(*reg.offset, labels);
                    },

                    [&data, &labels] (const ops::AddressIndex& reg) {
                        std::int16_t tmp = 0;
                        if (reg.offset.get())
                        {
                            tmp = momiji::resolveAST(*reg.offset, labels);
                        }

                        tmp &= 0x00FF;

                        data.cnt = 2;
                        data.val = (reg.othreg << 12) | tmp;
                    },

                    [&data, &labels, size] (const ops::Immediate& immediate) {
                        data.cnt = tobyte[size];
                        data.val = momiji::resolveAST(*immediate.value, labels);
                    },

                    [&data, &labels] (const ops::AbsoluteShort& addr) {
                        data.cnt = 2;
                        data.val = momiji::resolveAST(*addr.value, labels);
                    },

                    [&data, &labels] (const ops::AbsoluteLong& addr) {
                        data.cnt = 4;
                        data.val = momiji::resolveAST(*addr.value, labels);
                    }
                }, op);
            // clang-format on
        }
    }

    bool discriminateShifts(const ParsedInstruction& instr)
    {
        const auto& op = instr.operands[0];

        return matchOperand<operands::DataRegister>(op) ||
               matchOperand<operands::Immediate>(op);
    }
} // namespace momiji
