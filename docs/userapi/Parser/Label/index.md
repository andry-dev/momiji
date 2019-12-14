---
layout: class
title: momiji::Label
in-header: <momiji/Parser.h>
declaration: struct Label
description: Vocabulary type to describe a label.

fields:
    'string':
        type: std::string

    'idx':
        type: std::int64_t
        description: Location where the label is located the source code.
        default: 0

    'nameHash':
        type: std::uint32_t
        description: Hash of the name of the label.
        default: 0
---
