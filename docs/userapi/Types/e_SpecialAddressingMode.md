---
layout: enum
title: momiji::SpecialAddressingMode
in-header: <momiji/Types.h>
declaration: 'enum class SpecialAddressingMode : std::uint8_t'
description: |
    Enum describing the __bit__ representation of special addressing modes.

values:
    - name: ProgramCounterOffset
      description: The representation for a program counter with offset (0b010).

    - name: ProgramCounterIndex
      description: The representation for a program counter with index (0b011).

    - name: AbsoluteShort
      description: The representation for an absolute short (0b000).

    - name: AbsoluteLong
      description: The representation for an absolute long (0b001).

    - name: Immediate
      description: The representation for an immediate value (0b100).
---

This enum is used by the compiler and the decoder to properly encode information
about operands and/or "special" addressing modes.

### Future

This should be renamed to `BitSpecialAddressingMode`,
`LowLevelSpecialAddressingMode` or something similar, to indicate that it's
based on the bit representation of a special addressing mode.
