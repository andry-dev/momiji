#pragma once

#include <Parser.h>

#include "Combinators.h"
#include <expected.hpp>
#include <string_view>

namespace momiji::details
{
    using parserfn_t = momiji::parser_metadata (*)(std::string_view,
                                                   momiji::ParsedInstruction&);

    momiji::parser_metadata parseMove(std::string_view,
                                      momiji::ParsedInstruction&);

    momiji::parser_metadata parseMoveQ(std::string_view,
                                       momiji::ParsedInstruction&);

    momiji::parser_metadata parseAdd(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseSub(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseMuls(std::string_view,
                                      momiji::ParsedInstruction&);
    momiji::parser_metadata parseMulu(std::string_view,
                                      momiji::ParsedInstruction&);
    momiji::parser_metadata parseDivs(std::string_view,
                                      momiji::ParsedInstruction&);
    momiji::parser_metadata parseDivu(std::string_view,
                                      momiji::ParsedInstruction&);

    momiji::parser_metadata parseSwap(std::string_view,
                                      momiji::ParsedInstruction&);
    momiji::parser_metadata parseExg(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseOr(std::string_view,
                                    momiji::ParsedInstruction&);
    momiji::parser_metadata parseAnd(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseXor(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseNot(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseCmp(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseTst(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseJmp(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseJsr(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBra(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBsr(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseRts(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseBlt(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBle(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBgt(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBge(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBeq(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseBne(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseAsl(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseAsr(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseLsl(std::string_view,
                                     momiji::ParsedInstruction&);
    momiji::parser_metadata parseLsr(std::string_view,
                                     momiji::ParsedInstruction&);

    momiji::parser_metadata parseDeclare(std::string_view,
                                         momiji::ParsedInstruction&);
    momiji::parser_metadata parseHcf(std::string_view,
                                     momiji::ParsedInstruction&);
} // namespace momiji::details
