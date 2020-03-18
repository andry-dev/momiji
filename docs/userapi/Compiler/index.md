---
layout: library
title: Compiler
brief: |
    Component dedicated to compiling a valid piece of code to an executable
    binary.

toc:
    - 'Notes'
    - 'Basic usage example'
---

This component allows the user to compile the output of the parser ([`momiji::ParsingInfo`]({{ '/userapi/Parser/ParsingInfo' | relative_url }})) to an emulator-specific binary file.


The binary file generated is a blob of executable memory and, if needed, a data
section is included for variable declarations.
Both of these sections are delimited by markers, please refer to
[`momiji::Memory`]({{ '/userapi/Memory' | relative_url }}) for more information about them.

The generated executable does not contain any metadata about where each of the
sections begin and end, that functionality will be added soon.

### Notes

 *  The compiler at the moment __does not__ check that the parsed program is
    valid, that means that an ill-formed program can be passed to the compiler
    and it will attempt to compile it anyway, resulting in an ill-formed
    executable.

    At the moment, this happens because the compiler _expects_ the output of
    the parser to be __always__ valid, since there is no public-facing function
    to validate _again_ the output of the parser.
    Work is being done to provide such a function.

    This situation may arise if the user tries to modify directly the result of
    the parser by injecting instructions: by doing so an invalid sequence of
    instructions can be created.

 *  The compiler __does not__ try to optimize the input code and it never will.
    What is passed in is exactly what will be generated.

### Basic usage example

```cpp
auto res = momiji::parse(/* ... */);

if (!res) { /* ... */ }

auto parsingInfo = *res;

momiji::ExecutableMemory compiledBinary = momiji::compile(parsingInfo);
```
