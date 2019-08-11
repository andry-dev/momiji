#pragma once

#include <Parser.h>

#include "Combinators.h"
#include <expected.hpp>
#include <string_view>

namespace momiji::details
{
#if 0
    namespace v1
    {
        using parserfn_t = parser_metadata (*)(std::string_view str,
                                               momiji::Instruction&,
                                               LabelInfo&);

        parser_metadata
        parseMove(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseAdd(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseSub(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseMuls(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseMulu(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseDivs(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseDivu(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseSwap(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseExg(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseOr(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseAnd(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseXor(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseNot(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseCmp(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseTst(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseJmp(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseJsr(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBra(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBsr(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseRts(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseBlt(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBle(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBgt(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBge(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBeq(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseBne(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata
        parseAsl(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseAsr(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseLsl(std::string_view str, Instruction& instr, LabelInfo&);
        parser_metadata
        parseLsr(std::string_view str, Instruction& instr, LabelInfo&);

        parser_metadata parseDeclare(std::string_view str,
                                     Instruction& instr,
                                     LabelInfo& labels);
        parser_metadata
        parseHcf(std::string_view str, Instruction& instr, LabelInfo& labels);
    } // namespace v1

#endif

    inline namespace v2
    {
        using parserfn_t = momiji::v2::parser_metadata (*)(
            std::string_view, momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseMove(std::string_view,
                                              momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseMoveQ(std::string_view,
                                               momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseAdd(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseSub(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseMuls(std::string_view,
                                              momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseMulu(std::string_view,
                                              momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseDivs(std::string_view,
                                              momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseDivu(std::string_view,
                                              momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseSwap(std::string_view,
                                              momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseExg(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseOr(std::string_view,
                                            momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseAnd(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseXor(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseNot(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseCmp(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseTst(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseJmp(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseJsr(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBra(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBsr(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseRts(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseBlt(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBle(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBgt(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBge(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBeq(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseBne(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata parseAsl(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseAsr(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseLsl(std::string_view,
                                             momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseLsr(std::string_view,
                                             momiji::v2::ParsedInstruction&);

        momiji::v2::parser_metadata
        parseDeclare(std::string_view, momiji::v2::ParsedInstruction&);
        momiji::v2::parser_metadata parseHcf(std::string_view,
                                             momiji::v2::ParsedInstruction&);
    } // namespace v2
} // namespace momiji::details
