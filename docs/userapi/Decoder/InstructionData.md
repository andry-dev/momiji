---
layout: class
title: momiji::InstructionData
in-header: <momiji/Decoder.h>
declaration: struct InstructionData
description: |
    A compact class representing the data inside an instruction.
    This class is filled in by the decoder and it's used in the implementation
    of the instruction to retrieve the correct parameters.

fields:
    'size':
        type: std::int8_t
        description: |
            The size (as in, "data type") of the instruction.

            * 0 = Byte
            * 1 = Word
            * 2 = Long

            **TO BE CHANGED**
        default: 2

    'operandType':
        description: |
            An array of two operands for the instruction.
            The value inside `OperandType` is specific to the instruction.
        type: std::array<momiji::OperandType, 2>

    'addressingMode':
        type: std::array<momiji::SpecialAddressingMode, 2>
        description: |
            An array of two addressing modes for the instruction.
            The value inside `SpecialAddressingMode` is specific to the
            instruction.
---
