---
layout: class
title: momiji::errors::OperandTypeMismatch
in-header: <momiji/Parser.h>
description: |
    Error triggered when an instruction requires a type of operand different
    from the one provided.
declaration: struct OperandTypeMismatch

fields:
    'acceptedOps':
        type: std::vector<momiji::ParserOperands>
        description: Vector of accepted operands for that instruction.

    'inputOp':
        type: momiji::ParserOperand
        description: The input operand type found.

    'num':
        type: std::int8_t
        description: The number of the operand (0 = first, ...).
        default: 0
---
