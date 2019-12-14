---
layout: class
title: momiji::objects::MathASTNode
in-header: <momiji/Parser.h>
description: A node in an AST
declaration: struct MathASTNode

fields:
    value:
        type: std::variant<Label, Number, MathOperator>
        description: The value of the node
---
