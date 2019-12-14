---
layout: class
title: momiji::objects::MathOperator
in-header: <momiji/Parser.h>
declaration: struct MathOperator
description: A binary operator that applies to two AST nodes
fields:
    left:
        type: std::unique_ptr<momiji::objects::MathASTNode>
    right:
        type: std::unique_ptr<momiji::objects::MathASTNode>
    type:
        type: MathOperator::Type
---
