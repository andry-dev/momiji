#pragma once

#include <vector>
#include <Parser.h>

namespace momiji
{
    std::vector<std::uint8_t>
    compile(const std::vector<momiji::Instruction>& instructions);
}
