---
layout: function
title: momiji::extractASTValue
description: Extracts the AST value from an ASTOperand.
in-header: <momiji/Parser.h>

overloads:
    'std::int32_t extractASTValue(const Operand& operand, const LabelInfo& labels)':
        arguments:
            - name: operand
              type: const momiji::Operand&
              description: |
                The operand of which the function should resolve the AST.
            - name: labels
              type: const momiji::LabelInfo&
              description: |
                The labels present in the code.
        return: |
            The register value in the operand if it is a RegOperand, zero
            otherwise.
---
This function takes any [Operand]({{ '/userapi/Parser/Operand' | relative_url }}): if it is a
[ASTOperand]({{ 'ASTOperand' | relative_url }}) then it will resolve the AST and return the computed
value, else it will return zero.

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
