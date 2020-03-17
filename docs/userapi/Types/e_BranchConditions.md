---
layout: enum
title: momiji::BranchConditions
in-header: <momiji/Types.h>
declaration: 'enum class BranchConditions : std::uint8_t'
description: |
    Enum representing possible branch conditions for a conditional branch. (See
    [`InstructionType::BranchCondition`](e_InstructionType).)

    The values here are the exact same bit patterns (required by the spec) used
    when compiling or decoding an instruction.

values:
    - name: True
      description: Always true (0b0000).

    - name: False
      description: Always false (0b0001).

    - name: Higher
      description: |
        Equivalent to `bhi` (0b0010).

        **(unimplemented)**

    - name: LowerSame
      description: |
        Equivalent to `bls` (0b0011).

        **(unimplemented)**

    - name: CarryClear
      description: |
        Equivalent to `bcc` (0b0100).

        **(unimplemented)**

    - name: CarrySet
      description: |
        Equivalent to `bcs` (0b0101).

        **(unimplemented)**

    - name: NotEqual
      description: Equivalent to `bne` (0b0110).

    - name: Equal
      description: Equivalent to `beq` (0b0111).

    - name: OverClear
      description: |
        Equivalent to `bvc` (0b1000).

        **(unimplemented)**

    - name: OverSet
      description: |
        Equivalent to `bvs` (0b1001).

        **(unimplemented)**

    - name: Plus
      description: |
        Equivalent to `bpl` (0b1010).

        **(unimplemented)**

    - name: Minus
      description: |
        Equivalent to `bmi` (0b1011).

        **(unimplemented)**

    - name: GreaterEq
      description: Equivalent to `bge` (0b1100).

    - name: LessThan
      description: Equivalent to `blt` (0b1101).

    - name: GreaterThan
      description: Equivalent to `bgt` (0b1110).

    - name: LessEq
      description: Equivalent to `ble` (0b1111).
---
