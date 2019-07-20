#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <momiji/Memory.h>

namespace utils
{
    using ProgramArguments = std::vector<std::string_view>;

    inline ProgramArguments convArgs(int argc, const char** argv)
    {
        ProgramArguments args(argc - 1);

        for (int i = 1; i < argc; ++i)
        {
            const auto len = std::strlen(argv[i]);

            args[i - 1] = { argv[i], len };
        }

        return args;
    }

    inline std::string readFile(std::string_view path)
    {
        FILE* file = std::fopen(path.data(), "r");
        std::fseek(file, 0, SEEK_END);
        const auto length = std::ftell(file);

        std::string content(length, '\0');

        std::fseek(file, 0, SEEK_SET);
        std::fread(content.data(), 1, length, file);
        std::fclose(file);

        return content;
    }

    inline momiji::ExecutableMemory readBinary(std::string_view path)
    {
        FILE* file = std::fopen(path.data(), "r");

        std::fseek(file, 0, SEEK_END);
        const auto length = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);

        momiji::ExecutableMemory mem { length };

        std::fread(mem.data(), 1, length, file);
        std::fclose(file);

        return mem;
    }

    inline void writeFile(std::string_view path,
                          momiji::ExecutableMemoryView memory)
    {
        std::fstream file { path.data(), std::ios::binary | std::ios::out };
        file.write(reinterpret_cast<char*>(memory.data()), memory.size());
        file.close();
    }

} // namespace utils