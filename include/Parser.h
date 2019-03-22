#pragma once

#include "Types.h"
#include "CPU.h"
#include <array>
#include <string>
#include <vector>
#include <string_view>

#include "expected.hpp"

namespace momiji
{
    struct operand;
    struct instruction;

    using instr_fn_t = momiji::cpu_t(*)(momiji::cpu_t,
                                        const instruction&);

    struct operand
    {
        std::int32_t value;
        operand_type operandType;
        register_type registerType;
    };

    struct instruction
    {
        std::array<operand, 3> operands;

        instruction_type instructionType;
        data_type dataType;
        std::int8_t numOperands;

        instr_fn_t executefn;
    };

    struct parser_error
    {
        int line{0};
        int column{0};

        enum class error_type
        {
            NoInstructionFound,
            WrongInstruction,
            WrongOperandType,
            UnexpectedCharacter,
        } errorType;
    };

    nonstd::expected<momiji::instruction, parser_error> readInstruction(const std::string& str);
}
