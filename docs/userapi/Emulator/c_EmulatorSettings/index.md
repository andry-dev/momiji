---
layout: class
title: momiji::EmulatorSettings
in-header: "<momiji/Emulator.h>"
description: A class which contains settings for the Emulator
brief: Settings for the Emulator
declaration: struct EmulatorSettings
fields:
    programStart:
        type: std::int32_t
        description: Offset where the binary should start
        default: 0
    dataSectionOffset:
        type: std::int32_t
        description: Offset where the data section should start
        default: -1
    retainStates:
        type: RetainStates
        description: |
            Whether the emulator should retain system states during runs
        default: Always
    parserSettings:
        type: ParserSettings
        description: Settings passed to the parser
        default: __unspecified__
---
