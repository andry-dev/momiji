---
layout: default
title: momiji - Main
toc:
    - 'Use-case'
    - 'Building'
    - 'Tutorials'
    - 'Guides'
    - 'Public API Reference'
    - 'Private API Reference'
    - 'Troubleshooting'
    - 'Contributing'
---

# momiji documentation

momiji is a cross-platform C++ library (and a set of tools) used to emulate m68k
programs.
The library includes functions for parsing, compiling and executing m68k code,
there are also GUIs and CLI tools that wrap some of the common library's
features.

## Use-case

Maybe you need an embeddable and easy to use Emulator class:

```cpp
#include <momiji/Emulator.h>
#include <iostream>

int main()
{
    momiji::Emulator emu{};

    emu.newState(
        "move.w #123, d0 \n"
        "add.w #11, d0"
    );

    emu.step(); // Executes the move
    emu.step(); // Executes the add

    const auto& sys = emu.getStates().back();

    std::cout << sys.cpu.dataRegisters[0].raw() << '\n';
}
```

Or maybe you want to inspect the instructions in a program:

```cpp
#include <momiji/Parser.h>
#include <algorithm>

int main()
{
    auto parsingResult = momiji::parse(
        "move.w #123, d0\n"
        "move.l #1, d1\n"
        "add.l #1, a0"
    );

    if (!parsingResult)
    {
        // Error during parsing

        // Get the error and handle it in some way
        auto error = parsingResult.error();

        // ...

        return 1;
    }

    auto parsingInfo = *parsingResult;

    // Count the number of instructions with the '.l' data type
    const auto longs = std::count_if(
        std::begin(parsingInfo.instructions),
        std::end(parsingInfo.instructions),
        [] (const auto& x) {
            return x.dataType == momiji::DataType::Long;
        }
    )

    std::cout << longs << '\n';


    // Count the moves
    const auto moves = std::count_if(
        std::begin(parsingInfo.instructions),
        std::end(parsingInfo.instructions),
        [] (const auto& x) {
            return x.instructionType == momiji::InstructionType::Move;
        }
    );

    std::cout << moves << '\n';


    // How many labels there are in the code?
    std::cout << parsingInfo.labels.size() << '\n';
}
```

Whatever you may need, the library probably got you covered.
If you think something is missing, please
[open an issue](https://github.com/andry-dev/momiji/issues).

## Building

The first thing you may want to do is [build]({{ '/building' | relative_url }}) the library for your OS.

## Tutorials

If you are new, this is the place for you. See the [tutorials]({{ '/tutorials' | relative_url }}).

## Guides

## Public API Reference

[Here]({{ '/userapi' | relative_url }}) you can find the public API reference for the library.

## Private API Reference

Do you want to know how it all works or contribute? [Here]({{ '/devapi' | relative_url }}) you can find
the reference for the internal API of the library.

## Troubleshooting

## Contributing

Feel free to
[open an issue on Github](https://github.com/andry-dev/momiji/issues) or
[a PR](https://github.com/andry-dev/momiji/pull-requests)
