---
layout: class
title: momiji::CompilerSettings
in-header: <momiji/Compiler.h>
declaration: struct CompilerSettings
description: A class representing settings for the compiler.

fields:
    'programStart':
        type: std::int32_t
        description: |
            An offset indicating where the program should start relative to the
            initial byte of the binary.
        default: 0
---
