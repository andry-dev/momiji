---
layout: class
title: momiji::ParsingInfo
in-header: <momiji/Parser.h>
declaration: struct ParsingInfo
description: |
    Output of the parser describing all the found instructions and labels.

fields:
    'instructions':
        type: std::vector<momiji::ParsedInstruction>

    'labels':
        type: momiji::LabelInfo
---
