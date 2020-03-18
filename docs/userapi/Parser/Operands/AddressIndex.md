---
layout: class
title: momiji::operands::AddressIndex
in-header: <momiji/Parser.h>
declaration: struct AddressIndex
description: |
    An operand of the form `(X, Y*, a*)` where '\*' is any number from 0 to 7,
    'Y' is any data or address register and 'X' is any compile-time expression.
    This references the address inside the address register plus the value
    inside the register 'Y\*' plus the value of 'X'.
    It's also possible to omit 'X', which results in an expression of the form
    `(Y*, a*)`.
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
    othreg:
        type: std::int32_t
        description: |
            The number of the other register ('Y*').
---

In `(X, Y*, a*)`, 'X' is any compile-time expression, see
[AST objects]({{ '/userapi/Parser/Objects' | relative_url }}) for some examples.
