---
layout: class
title: 'momiji::StatusRegister'
description: |
    A class representing the status register.
    The status register is set accordingly when executing a `cmp`, `add`, `sub`,
    `mul` or `div`.
in-header: <momiji/System.h>
declaration: struct StatusRegister

fields:
    'extend':
        type: _Implementation-defined_
        description: |
            Also called 'E' or 'X', it signals if a sign extension happened.

            **Unimplemented.**
        default: 0

    'negative':
        type: _Implementation-defined_
        description: |
            Also called 'N', it signals if the resulting number is negative.
        default: 0

    'zero':
        type: _Implementation-defined_
        description: |
            Also called 'Z', it signals if the resulting number is zero.
        default: 0

    'overflow':
        type: _Implementation-defined_
        description: |
            Also called 'V', it signals if an overflow happened.
        default: 0

    'carry':
        type: _Implementation-defined_
        description: |
            Also called 'C', it signals if a carry happened.

            **Unimplemented.**
        default: 0
---

### Status of implementation

When a status is "unimplemented" it means that no instruction changes that
value.

This is not by design! This just means that sooner or later it will be set by
an instruction.
