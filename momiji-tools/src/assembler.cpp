#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <momiji/Compiler.h>
#include <momiji/Parser.h>

constexpr std::string_view usage =
    "USAGE: momiji-as input_file [output_file]\n"
    "If output_file is not specified, the output file is named after the first "
    "one but with the extension '.mb'/.\n"
    "The program doesn't care about the input file's extension.\n"
    "\n"
    "Example: \n"
    "  'momiji-as test.m68k' yields the compiled file 'test.mb'.\n";

int main(int argc, const char** argv)
{
    namespace fs = std::filesystem;

    auto args = utils::convArgs(argc, argv);

    if (args.size() < 1 || args.size() > 2)
    {
        std::cout << usage;
        return 1;
    }

    std::string sourceCode = utils::readFile(args[0]);

    auto parsedInstr = momiji::parse(sourceCode);
    if (!parsedInstr)
    {
        using momiji::ParserError;

        const auto error = parsedInstr.error();
        std::string errorStr =
            "Parsing error at line " + std::to_string(error.line) + ": ";

        switch (error.errorType)
        {
        case ParserError::ErrorType::NoInstructionFound:
            errorStr += "no instruction found.";
            break;

        case ParserError::ErrorType::NoLabelFound:
            errorStr +=
                "no label found. Are you sure you typed everything correctly?";
            break;

        case ParserError::ErrorType::WrongInstruction:
            errorStr += "the instruction name is wrong.";
            break;

        case ParserError::ErrorType::WrongOperandType:
            errorStr += "the operand type is not supported for the following "
                        "instruction.";
            break;

        case ParserError::ErrorType::WrongRegisterNumber:
            errorStr += "the register number is wrong (either less than 0 or "
                        "more than 7).";
            break;

        case ParserError::ErrorType::UnexpectedCharacter:
            errorStr += "unexpected character.";
            break;
        }

        errorStr += "\n\tContext: " + error.codeStr + "\n";

        std::cout << errorStr;

        return 1;
    }

    auto compiledInstr = momiji::compile(*parsedInstr);

    if (args.size() == 2)
    {
        utils::writeFile(args[1], compiledInstr);
    }
    else
    {
        auto newFile = fs::path { args[0] }.replace_extension("mb");
        utils::writeFile(newFile.string(), compiledInstr);
    }

    auto x = fs::current_path();

    return 0;
}
