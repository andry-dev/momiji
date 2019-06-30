#pragma once

#include <Parser.h>

#include "Combinators.h"
#include <expected.hpp>
#include <string_view>

namespace momiji::details
{
    using parserfn_t = parser_metadata (*)(std::string_view str,
                                           momiji::Instruction&, LabelInfo&);

    parser_metadata parseMove(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseAdd(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseSub(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseMuls(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseMulu(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseDivs(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseDivu(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseSwap(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseExg(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseOr(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseAnd(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseCmp(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseJmp(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBra(std::string_view str, Instruction& instr, LabelInfo&);

    parser_metadata parseBlt(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBle(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBgt(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBge(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBeq(std::string_view str, Instruction& instr, LabelInfo&);
    parser_metadata parseBne(std::string_view str, Instruction& instr, LabelInfo&);
}
