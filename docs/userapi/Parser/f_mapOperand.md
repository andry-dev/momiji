---
layout: function
title: momiji::mapOperand
in-header: <momiji/Parser.h>
brief: |
    Executes a functor to an [`momiji::Operand`]({{ '/userapi/Parser#typedefs' | relative_url }}) if
    it holds an `OperandType`.

overloads:
    'inline bool mapOperand(momiji::Operand& op, Fun&& fn)':
        description: |
            This function takes an Operand, a functor and a template
            (OperandType). It checks that the Operand is of type OperandType
            (which means the variant is storing that particular type), if it
            is it calls the functor with the Operand and returns true.
        template-arguments:
            - name: OperandType
              description: The expected operand type to match
            - name: Fun
              description: Functor with signature <code>void(momiji::Operand&)</code>
        arguments:
            - name: op
              type: momiji::Operand&
              description: The <code>Operand</code> of which to apply the function
            - name: fn
              type: Fun&&
              description: The functor to execute
        return: true if it could apply the functor, false otherwise.
---

### Examples

```cpp
momiji::Operand op = momiji::operands::DataRegister{ 2 };

// ...
// In some generic code down the line:

// This works
mapOperand<momiji::operands::DataRegister>(op, [] (momiji::operands::DataRegister& x) {
    std::cout << x.reg << '\n';
});

// This doesn't
mapOperand<momiji::operands::AddressRegister>(op, [] (momiji::operands::AddressRegister& x) {
    std::cout << x.reg << '\n';
});

// You can also use "auto" and friends, if you know what you're doing
mapOperand<momiji::operands::Immediate>(op, [] (auto&) {
    std::cout << "Matched an immediate!\n";
});
```
