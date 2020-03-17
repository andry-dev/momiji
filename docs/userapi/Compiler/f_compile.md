---
layout: function
title: momiji::compile
in-header: <momiji/Compiler.h>
description: Compiles the result of the parser to an executable program.

overloads:
    'momiji::ExecutableMemory compile(const momiji::ParsingInfo& parsingInfo, momiji::CompilerSettings settings)':
        arguments:
            - name: parsingInfo
              type: const momiji::ParsingInfo&
              description: |
                The result of the parser.
            - name: settings
              type: momiji::CompilerSettings
              description: |
                Settings for the compiler.
        return: The binary of the compiled program.

flags:
    - unstable-api
---

The resulting [`momiji::ExecutableMemory`](/userapi/Memory) contains the direct representation of the source
code in binary.

The supported way to interpret it is to pass the binary to the [Decoder](/userapi/Decoder) to decode a specific location.
