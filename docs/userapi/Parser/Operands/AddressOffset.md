---
layout: class
title: momiji::operands::AddressOffset
in-header: <momiji/Parser.h>
declaration: struct AddressOffset
description: |
    An operand in the form `(X, a*)` where '\*' is any number from 0 to 7 and
    'X' is any compile-time expression.
    This references the address inside an address register plus the value of 'X'.
fields:
    offset:
        type: std::unique_ptr<objects::MathASTNode>
        description: |
            The AST representing the value of the offset.
    reg:
        type: std::int32_t
        description: |
            The number of the address register.
            (eg: 0 is the first, 7 is the eight)
---

In `(X, a*)`, 'X' is any compile-time expression, see
[AST objects](/userapi/Parser/Objects) for some examples.
