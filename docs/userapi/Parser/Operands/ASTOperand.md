---
layout: interface
title: ASTOperand
description: An operand that has an AST which can be resolved.
fields:
    value:
        type: std::unique_ptr<momiji::objects::MathASTNode>
        description: |
            The root of the AST which should be resolved to get the value.
---
