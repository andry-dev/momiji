---
layout: enum
title: momiji::ParserOperand
in-header: "<momiji/Parser.h>"
brief: A _lightweight_ enum describing the possible operand types
description: |
    This enum describes possible operand types.
    It is primarily used for LIGHTWEIGHT __error__ checking, like if a type of operand is in a list of other operands.
    This is **NOT** meant to be used for storing operands with their information, for that you should use [`momiji::Operand`](/userapi/Parser#typedefs).
declaration: 'enum class ParserOperand : std::int8_t'
values:
    - name: DataRegister
    - name: AddressRegister
    - name: Address
    - name: AddressPre
    - name: AddressPost
    - name: AddressOffset
    - name: AddressIndex
    - name: Immediate
    - name: AbsoluteShort
    - name: AbsoluteLong
    - name: ProgramCounterOffset
    - name: ProgramCounterIndex
---

### Remarks

This is completely different from [`momiji::Operand`](/userapi/Parser#typedefs)!

When manipulating __VALUES OF OPERANDS__ you should use that one!

### Future

This is probably the worst name in the library.

When error-checking, some context is not _really_ needed, this means that some
space can be saved when storing an error: a typical class in
`momiji::operands::*` occupies 32 to 64 bits of memory, while a `ParserOperand`
takes just 8 bits.

An example of this is when the parser signals that an operand is invalid for a
particular instruction _and then returns the list of valid operands_. In that
case, what would be appropriate? Creating a mock ~64-bits wide `momiji::Operand`
to convey 8-bits of information is wasteful.
