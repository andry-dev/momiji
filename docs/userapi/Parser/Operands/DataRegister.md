---
layout: class
title: momiji::operands::DataRegister
description: |
    A data register in the form `d*` where '\*' is any number from 0 to 7.
fields:
    reg:
        type: std::int32_t
        description: 'The number of the register. (eg: 0 is the first, 7 is the eight)'
---

### Example

In the instruction `move.w #1, d0`, the second operand is a `DataRegister` with
the field `reg == 0`.

In `move.w d1, d7`, the first operand is a `DataRegister` with `reg == 1` and
the second is also a `DataRegister` but with `reg == 7`.
