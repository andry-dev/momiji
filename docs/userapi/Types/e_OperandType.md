---
layout: enum
title: momiji::OperandType
in-header: <momiji/Types.h>
declaration: 'enum class OperandType : std::uint8_t'
description: Enum describing the __bit__ representation of an operand.

values:
        - name: DataRegister
          description: The bit representation of a data register (0b000).

        - name: AddressRegister
          description: The bit representation of an address register (0b001).

        - name: Address
          description: The bit representation of an address (0b010).

        - name: AddressPost
          description: |
            The bit representation of an address with post-increment (0b011).

        - name: AddressPre
          description: |
            The bit representation of an address with pre-decrement (0b100).

        - name: AddressOffset
          description: |
            The bit representation of an address with an offset (0b101).

        - name: AddressIndex
          description: |
            The bit representation of an address with another register as an
            index (0b110).

        - name: ProgramCounterOffset
          description: |
            The bit representation for a "special" addressing mode (0b111).

        - name: ProgramCounterIndex
          description: |
            The bit representation for a "special" addressing mode (0b111).

        - name: AbsoluteShort
          description: |
            The bit representation for a "special" addressing mode (0b111).

        - name: AbsoluteLong
          description: |
            The bit representation for a "special" addressing mode (0b111).

        - name: Immediate
          description: |
            The bit representation for a "special" addressing mode (0b111).

flags:
    - bad-name
---

A value in this enum is a combination of _three_ bits that is used when
compiling or decoding an operand. The values here follow _exactly_ the spec.

The `ProgramCounterOffset`, `ProgramCounterIndex`, `AbsoluteShort`,
`AbsoluteLong` and `Immediate` have the same value because they are described in
another three bits reserved for
["special" addressing modes](e_SpecialAddressingMode).


### Future

This should be renamed to `BitAddressingMode`,
`LowLevelAdressingMode` or something similar, to indicate that it's based on the
bit representation of an addressing mode. This is not about operands in
particular but more on their representation.
