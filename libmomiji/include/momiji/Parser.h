#pragma once

#include "Types.h"

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <optional>

#include <gsl/span>

#include "expected.hpp"

#include <momiji/Parser/Errors.h>
#include <momiji/Parser/AST.h>
#include <momiji/Parser/Operands.h>
#include <momiji/Parser/Common.h>
#include <momiji/Parser/details.h>

namespace momiji
{
    struct ParserSettings
    {
        gsl::span<Breakpoint> breakpoints;
    };

    struct ParsedInstruction
    {
        ParsedInstruction()
            : operands(2)
        {
        }

        std::vector<Operand> operands;
        DataType dataType : 2;
        InstructionType instructionType : 6;
        BranchConditions branchCondition : 4;
        std::int32_t programCounter;
        std::int32_t sourceLine;
    };

    // TODO(andry): The name is bad
    struct ParsingInfo
    {
        std::vector<momiji::ParsedInstruction> instructions;
        momiji::LabelInfo labels;
    };

    using ParsingResult =
        nonstd::expected<momiji::ParsingInfo, momiji::ParserError>;

    momiji::ParsingResult parse(const std::string& str);
    momiji::ParsingResult parse(const std::string& str,
                                ParserSettings settings);

    std::optional<momiji::ParserError>
    sanitizeParsingInfo(const ParsingInfo& parsingInfo);


    momiji::ParserOperand convertOperand(const momiji::Operand& operand);


} // namespace momiji
