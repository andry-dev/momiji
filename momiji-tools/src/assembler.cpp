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
        std::cout << "Parsing error\n";
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
