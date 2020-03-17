---
layout: class
title: momiji::ParsedInstruction
in-header: <momiji/Parser.h>
declaration: struct ParsedInstruction
description: A struct representing a parsed instruction.

fields:
    'operands':
        type: std::vector<momiji::Operand>
        description: Vector of operands.

    'dataType':
        type: momiji::DataType
        description: The data type of the instruction.

    'instructionType':
        type: momiji::InstructionType
        description: The type of the instruction.

    'branchCondition':
        type: momiji::BranchConditions
        description: |
            If the instruction is a branch instruction, this describes the
            condition that the branch should use.

    'programCounter':
        type: std::int32_t
        description: The calculated program counter of the instruction.

    'sourceLine':
        type: std::int32_t
        description: |
            The line in the source code where the instruction is located.
---
