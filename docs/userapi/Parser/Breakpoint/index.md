---
layout: class
in-header: "<momiji/Parser.h>"
title: momiji::Breakpoint
brief: Breakpoint to be injected by the parser
description: |
    When a Breakpoint is passed to the parser (via an instance of [`ParserSettings`](/ParserSettings)) the parser injects an instruction of type [`InstructionType::Breakpoint`](/InstructionType) at the location the breakpoint points to.
declaration: struct Breakpoint
fields:
    source_line:
        type: std::int64_t
        description: The line number in the source file where an hardware breakpoint should be added
        default: 0
---
