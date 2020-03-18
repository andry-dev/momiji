---
layout: class
title: momiji::operands::AbsoluteLong
in-header: <momiji/Parser.h>
declaration: struct AbsoluteLong
description: |
    An operand of the form `X` where 'X' is any compile-time expression and the
    data size of the instruction is a Long (32-bit wide).
    The literal 32-bit value of 'X' is used.
fields:
    value:
        type: std::unique_ptr<objects::MathASTNode>
        description: |
            The AST representing the value of the immediate data
---

'X' is any compile-time expression, see
[AST objects]({{ '/userapi/Parser/Objects' | relative_url }}) for some examples.
