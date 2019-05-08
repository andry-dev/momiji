#include <Compiler.h>

#include "../Instructions/OpCodes.h"

#include <Utils.h>
#include <iostream>

namespace momiji
{
    namespace
    {
        constexpr std::array<std::uint8_t, 3> move_sizeconv = {
            0b01, // Byte (DataType::Byte)
            0b11, // Word (DataType::Word)
            0b10  // Long (DataType::Long)
        };
    }

    std::vector<std::uint8_t>
    compile(const std::vector<momiji::Instruction>& instructions)
    {
        std::vector<std::uint8_t> memory;

        for (const auto& instr : instructions)
        {
            union
            {
                std::uint16_t val;
                struct
                {
                    std::uint8_t lower;
                    std::uint8_t higher;
                };
            } opcode;

            std::uint8_t size = utils::to_val(instr.dataType);

            struct
            {
                union
                {
                    std::uint32_t val;
                    std::uint16_t arr16[2];
                    std::uint8_t arr8[4];
                };
                std::uint8_t cnt = 0;
            } additional_data;

            additional_data.val = 0;

            switch (instr.instructionType)
            {
            case InstructionType::Move: {
                opcode.val = codes::move;

                // This Move requires special handling of sizes.
                size = move_sizeconv[utils::to_val(instr.dataType)];

                // Byte size
                opcode.val |= (size << 12);

                // Destination
                std::uint32_t dstval = utils::to_val(instr.operands[1].operandType);;
                std::uint8_t dstmode = utils::to_val(instr.operands[1].specialAddressingMode);;

                switch (instr.operands[1].operandType)
                {
                case OperandType::AddressRegister: [[fallthrough]];
                case OperandType::DataRegister:
                    dstval = instr.operands[1].value & 0b0111;
                    dstmode = utils::to_val(instr.operands[1].operandType);
                    break;

                default:
                    break;
                }
                opcode.val |= (dstval & 0b0111) << 9;
                opcode.val |= (dstmode & 0b0111) << 6;

                // Source
                std::uint32_t srcval = utils::to_val(instr.operands[0].operandType);
                std::uint8_t srcmode = utils::to_val(instr.operands[0].specialAddressingMode);

                switch (instr.operands[0].operandType)
                {
                case OperandType::DataRegister: [[fallthrough]];
                case OperandType::AddressRegister:
                    srcmode = utils::to_val(instr.operands[0].operandType);
                    srcval = instr.operands[0].value & 0b0111;
                    break;

                case OperandType::Immediate:
                    switch (instr.operands[0].specialAddressingMode)
                    {
                    case SpecialAddressingMode::Immediate:
                        additional_data.val = instr.operands[0].value;
                        switch (instr.dataType)
                        {
                        case DataType::Byte:
                            additional_data.cnt = 1;
                            break;

                        case DataType::Word:
                            additional_data.cnt = 2;
                            break;

                        case DataType::Long:
                            additional_data.cnt = 4;
                            break;
                        }
                        break;

                    default:
                        break;
                    }

                default:
                    break;
                }

                opcode.val |= (srcmode & 0b0111) << 3;
                opcode.val |= (srcval & 0b0111);
            } break;

            default:
                break;
            }

            memory.push_back(opcode.higher);
            memory.push_back(opcode.lower);

            std::cout << "Adding bytes: "
                << std::to_string(additional_data.cnt)
                << " - "
                << additional_data.val << '\n';

            for (int i = 0; i < additional_data.cnt; ++i)
            {
                memory.push_back(additional_data.arr8[i]);
            }
        }

        return memory;
    }
}
