---
layout: class
title: 'momiji::traps::InvalidMemoryWrite'
description: A class that represents an invalid memory write.
in-header: <momiji/System.h>
declaration: struct InvalidMemoryWrite

fields:
    'address':
        type: 'std::int32_t'
        description: |
            The address where an invalid write occurred.

    'value':
        type: 'std::int32_t'
        description: |
            The value being written to the invalid memory location.
---
