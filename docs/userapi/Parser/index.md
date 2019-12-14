---
layout: library
title: Parser

typedefs:
    'momiji::Operand':
        definition: |
            ```
            std::variant<momiji::operands::DataRegister,
                         momiji::operands::AddressRegister,
                         momiji::operands::Address,
                         momiji::operands::AddressPre,
                         momiji::operands::AddressPost,
                         momiji::operands::AddressOffset,
                         momiji::operands::AddressIndex,
                         momiji::operands::Immediate,
                         momiji::operands::ProgramCounterOffset,
                         momiji::operands::ProgramCounterIndex,
                         momiji::operands::AbsoluteShort,
                         momiji::operands::AbsoluteLong>;
            ```
        description: |
            A discriminated union representing an operand in an instruction.

    'momiji::LabelInfo':
        definition: |
            ```cpp
            std::vector<momiji::Label>;
            ```
        description: The labels found in the source code.

    'momiji::ParsingResult':
        definition: |
            ```
            nonstd::expected<momiji::ParsingInfo,
                             momiji::ParserError>;
            ```
        description: |
            A value representing either the parsed data or an error.

toc:
    - Basic usage example
    - Handling errors
    - Inspecting instructions
    - ASTs
---

This component is probably the biggest in the library. It allows the user to
introspect most of the useful stuff generated during parsing.

### Basic usage example

The straightforward usage of this component is the following:

```cpp
std::string code = /* ... */;

// Parse the source code
momiji::ParsingResult res = momiji::parse(code);

// Check if there was an error during parsing
if (!res)
{
    momiji::ParserError error = res.error();

    // Handle error in some way

    return false;
}

// If there was no error, get the parsed instructions and labels
momiji::ParsingInfo parsingInfo = *res;

// Inspect the instructions
for (const auto& instruction : parsingInfo.instructions)
{
    // ...
}

// Inspect the labels
for (const auto& label : parsingInfo.labels)
{
    // ...
}
```

Most of the time, you may not need to go beyond handling a parsing error: almost
all the other classes here are for introspecting and representing every sort of
state.

You can manipulate the `momiji::ParsingInfo` struct to do any sort of fun stuff
(injecting instructions?) but, ultimately, the most useful thing you can do with
it is passing it to the [Compiler](/userapi/Compiler) to compile the
instructions and get an executable.

### Handling errors

The source code passed to the parser may be ill-formed. That's ok! The parser
will let you know by returning a `momiji::ParserError` and let you recover from
the error.

Handling it is fairly simple:

```cpp
auto res = momiji::parse(/* ... */);

if (!res)
{
    // Something happened

    auto error = res.error();

    // Obviously you can do whatever you want here, this is just an example

    std::cout << "Error happened at line " << std::to_string(error.line)
              << "when parsing string '" << error.codeStr << "'.\n";

    std::visit(error.errorType, asl::overload{
        [] (const momiji::errors::InvalidRegisterNumber& x) {
            std::cout << "Invalid register number '" << std::to_string(x.input)
                      << "'.\n";
        },

        [] (const momiji::errors::DataTypeMismatch& x) {
            // ...
        },

        [] (const momiji::errors::MissingCharacter& x) {
            // ...
        },

        // ...
    });

    return false;
}
```

To visit the variant containing the type of error you can make a functor with
the appropriate `operator()`s or use `asl::overload` with lambdas.

No direct function to translate error codes to string is provided because of
localization issues: an application may want to translate the strings
differently or word them differently, so I think there's no use in providing a
way to directly convert the errors to strings. You can make your own function
that calls `Qt::tr()` or whatever you have to handle translations.

### Inspecting instructions

Advanced users may need to inspect the vector of instructions and inject their
own (or run some sort of static analysis of a program). It's not that difficult,
provided that you know what you're doing.

```cpp
auto res = momiji::parse(/* ... */);

if (!res)
{
    /* ... */
}

auto parsingInfo = *res;

// Print information about each instruction
// The stuff with "custom::" is your own stuff, it's not provided by the library
for (const momiji::ParsedInstruction& instr : parsingInfo.instructions)
{
    std::cout << "Instruction: " << custom::to_string(instr.instructionType) << "\n"
              << "Operands: " << custom::to_string(instr.operands) << "\n"
              << "Virtual PC: " << instr.programCounter << "\n"
              << "Source line: " << instr.sourceLine << "\n";

    if (instr.instructionType == momiji::InstructionType::BranchCondition)
    {
        std::cout << "Branch condition: "
                  << custom::to_string(instr.branchCondition) << "\n";
    }
}

// Inject an "add.w d0, d1" in the third position
{
    auto it = parsingInfo.instructions.begin() + 2;

    momiji::ParsingInstruction instr;

    instr.instructionType = momiji::InstructionType::Add;
    // Two operands are ALWAYS provided for you
    instr.operands[0] = momiji::operands::DataRegister{0};
    instr.operands[1] = momiji::operands::DataRegister{1};

    instr.dataType = momiji::DataType::Word;

    parsingInfo.instructions.insert(it, instr);
}
```

Similar stuff can be done to inspect labels.

### ASTs

An AST describes a compile-time mathematical expression which can be resolved
to yield a 32-bit signed integer.
An expression can be made of 32-bit signed integer numbers
(..., -2, -1, 0, 1, 2, ...), labels, binary operators and parenthesis.

When parsing `move.w #123, some_label + 4` two ASTs
are created: one for the `#123` immediate and the other for the
`some_label + 4` address.

It's possible to build complex expressions: `move.w #(2 * 4 * a_label) / 2, d0`;
in that case the first operand is an immediate value corresponding to 4 times
the location of a label.

A generated AST _is not optimized_, which means that the parser doesn't try to
reduce expressions (for example, reduce `2 + 2` from three nodes to just one
with the value `4`).
An optimizer is out of the scope of the library, you can write your own if you
need to.

In the future callable functions may be implemented (to calculate sin, cos, pow,
etc... at compile-time) along with miscellaneous utilities (eg: an `!here`
expression that would yield the current source location of the instruction).
