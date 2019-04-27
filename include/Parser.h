#pragma once

#include "Types.h"
#include <array>
#include <string>
#include <vector>
#include <string_view>

#include "expected.hpp"

#include "System.h"

namespace momiji
{
    struct instruction;

    using instr_fn_t = momiji::system(*)(momiji::system,
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
            Comment,
        } errorType;
    };

    using parsing_result = nonstd::expected<std::vector<momiji::instruction>, parser_error>;


    nonstd::expected<momiji::label, parser_error> readLabel(const std::string& str);
    nonstd::expected<momiji::instruction, parser_error> readInstruction(const std::string& str);

    momiji::parsing_result parse(const std::string& str);


}
