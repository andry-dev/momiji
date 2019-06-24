#pragma once

#include "Combinators.h"
#include <expected.hpp>
#include <string_view>

namespace momiji
{
    namespace details
    {
        using ResultType = nonstd::expected<
            parser_metadata,
            ParserError
        >;
        using parserfn_t = ResultType(*)(std::string_view str, momiji::Instruction&, LabelInfo&);
    }

    details::ResultType parseMove(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseAdd(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseSub(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseMuls(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseMulu(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseDivs(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseDivu(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseSwap(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseExg(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseOr(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseAnd(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseCmp(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseJmp(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBra(std::string_view str, Instruction& instr, LabelInfo&);

    details::ResultType parseBlt(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBle(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBgt(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBge(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBeq(std::string_view str, Instruction& instr, LabelInfo&);
    details::ResultType parseBne(std::string_view str, Instruction& instr, LabelInfo&);
}
