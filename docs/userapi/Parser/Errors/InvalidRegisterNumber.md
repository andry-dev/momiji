---
layout: class
title: momiji::errors::InvalidRegisterNumber
in-header: <momiji/Parser.h>
description: |
    Error triggered when an invalid number is used to index a register.
declaration: struct InvalidRegisterNumber

fields:
    'input':
        type: std::int32_t
        description: The input register number.
---

### Example

`move.w #123, d12` will invoke this error because there's no 12nd data register.
