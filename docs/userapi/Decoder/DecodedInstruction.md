---
layout: class
title: momiji::DecodedInstruction
in-header: <momiji/Decoder.h>
declaration: struct DecodedInstruction
description: |
    A class representing a decoded instruction generated from the decoder.

fields:
    'data':
        type: momiji::InstructionData
        description: |
            The data associated with the instruction (eg: the operands, which
            size it is, which addressing mode is it using, etc...).

    'string':
        type: momiji::InstructionString
        description: |
            The string representation of the instruction.
            This is reconstructed by the decoder.

    'exec':
        type: momiji::DecodedInstructionFn
        description: |
            A pointer to the function implementing that instruction.
---
