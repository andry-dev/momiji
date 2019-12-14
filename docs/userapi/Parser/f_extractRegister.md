---
layout: function
title: momiji::extractRegister
description: Extracts the register number from a RegOperand.
in-header: <momiji/Parser.h>

overloads:
    'std::int32_t extractRegister(const Operand& operand)':
        arguments:
            - name: operand
              type: const momiji::Operand&
              description: |
                The operand of which the function should extract the value of
                the register.
        return: |
            The register value in the operand if it is a RegOperand, zero
            otherwise.
---

This function takes any [Operand](../Operand): if it is a
[RegOperand](RegOperand) then it will return the stored register number, else
it will return zero.

This is mostly useful in generic code, otherwise one should have to switch on
the Operand variant, and that would be annoying.

### Example

Instead of this:

```cpp
momiji::Operand op = /* ... */;

// ...
// A call stack later

std::int32_t regval = 0;

std::visit(op, asl::overloaded{
    [&regval] (const momiji::operands::DataRegister& op) {
        regval = op.reg;
    },

    [&regval] (const momiji::operands::AddressRegister& op) {
        regval = op.reg;
    },

    [&regval] (const momiji::operands::Address& op) {
        regval = op.reg;
    },

    // Repeat for all the other classes that expose 'reg'
    // ...

    [] (auto&&) {
        // Do nothing if none of the above
    }
});

```

You can write this instead:

```cpp
momiji::Operand op = /* ... */;

const std::int32_t regval = momiji::extractReg(op);
```
