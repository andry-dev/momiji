#include "./Directives.h"

#include "./Combinators.h"

namespace momiji::details
{
    momiji::parser_metadata parseSection(std::string_view str,
                                         momiji::ParsedInstruction& instr)
    {
        auto tryCodeSection = SeqNext(Whitespace(), Str("code"))(str);

        if (tryCodeSection.result)
        {
            instr.instructionType = InstructionType::CodeMarker;
            return tryCodeSection;
        }

        auto tryDataSection = SeqNext(Whitespace(), Str("data"))(str);

        if (tryDataSection.result)
        {
            instr.instructionType = InstructionType::DataMarker;
        }

        return tryDataSection;
    }
} // namespace momiji::details
