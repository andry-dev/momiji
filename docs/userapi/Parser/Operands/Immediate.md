---
layout: class
title: momiji::operands::Immediate
in-header: <momiji/Parser.h>
declaration: struct Immediate
description: |
    An operand of the form `#X` where 'X' is any compile-time expression.
    The literal value of 'X' is used.
fields:
    value:
        type: std::unique_ptr<objects::MathASTNode>
        description: |
            The AST representing the value of the immediate data
---

'X' is any compile-time expression, see
[AST objects]({{ '/userapi/Parser/Objects' | relative_url }}) for some examples.
