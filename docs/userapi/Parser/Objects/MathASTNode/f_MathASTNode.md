---
layout: method
ctor: true
title: MathASTNode
in-header: <momiji/Parser.h>

overloads:
    'MathASTNode()':
        description: Default constructor

    'MathASTNode(Label&& label)':
        arguments:
            - name: label
              type: momiji::objects::Label&&
        description: Initializes the node with a Label

    'MathASTNode(Number&& num)':
        arguments:
            - name: num
              type: momiji::objects::Number&&
        description: Initializes the node with a Number

    'MathASTNode(MathOperator&& op)':
        arguments:
            - name: op
              type: momiji::objects::MathOperator&&
        description: Initializes the node with an operator
---
