#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <momiji/Compiler.h>
#include <momiji/Parser.h>

#include <asl/types>
#include <variant>

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

    if (args.empty() || args.size() > 2)
    {
        std::cout << usage;
        return 1;
    }

    std::string sourceCode = utils::readFile(args[0]);

    auto parsedInstr = momiji::parse(sourceCode);

    if (!parsedInstr)
    {
        using momiji::ParserError;

        auto error = parsedInstr.error();
        std::string errorStr =
            "Parsing error at line " + std::to_string(error.line) + ": ";

        using namespace momiji::errors;
        // clang-format off
        std::visit(asl::overloaded {
            [&](const UnknownError& /* unused */) {
                errorStr += "unknown error.";
            },

            [&](const NoInstructionFound& par) {
                errorStr += "instruction \"" + par.inputString +
                            "\" not found";

                if (!par.alternatives.empty())
                {
                    errorStr += "\nAlternatives:\n";
                    for (const auto& x : par.alternatives)
                    {
                        errorStr += x + '\n';
                    }
                }
            },

            [&](const NoLabelFound& par) {
                errorStr += "label \"" + par.label + "\" not found.";
            },

            [&](const DataTypeMismatch& par) {
                errorStr += "data type " + utils::toString(par.inputDataType) +
                            " is not valid for this instruction.";

                if (!par.acceptedDataTypes.empty())
                {
                    errorStr += "\nAccepted data types: ";
                    errorStr += utils::contToString(par.acceptedDataTypes);
                }
            },

            [&](const OperandTypeMismatch& par) {
                errorStr += "operand type " + utils::toString(par.inputOp) +
                            " is not valid for this instruction.";

                if (!par.acceptedOps.empty())
                {
                    errorStr += "\nAccepted operand types: ";
                    errorStr += utils::contToString(par.acceptedOps);
                }
            },

            [&](const InvalidRegisterNumber& par) {
                errorStr += "Invalid register number " +
                            std::to_string(par.input);
            },

            [&](const UnexpectedCharacter& par) {
                errorStr += "Unexpected character '" +
                            std::string{par.character} + "'.";
            },

            [&](const MissingCharacter& par) {
                errorStr += "Missing a '" +
                            std::string{par.character} + "'.";
            },

            [&](const UnknownOperand& /* unused */) {
                errorStr += "Unknown operand, are you sure the syntax is valid?";
            },

            [&](const UnexpectedSectionContent& par) {
                switch (par.section)
                {
                case momiji::ParserSection::Code:
                    errorStr += "Unexpected variable definition in code section";
                    break;

                case momiji::ParserSection::Data:
                    errorStr += "Unexpected executable instruction in data section";
                    break;
                }
            }
        }, error.errorType);
        // clang-format on

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
