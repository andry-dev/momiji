---
layout: library
title: Decoder
description: |
    Component dedicated to decoding an instruction (located at a specific
    offset) from an executable program.

typedefs:
    'InstructionString':
        definition: "std::string"
        description: Describes the original "string" form of an instruction.

    'DecodedInstructionFn':
        definition: |
            `momiji::System (*)(momiji::System&, const momiji::InstructionData&)`
        description: |
            A function pointer to the implementation of a specific instruction.

toc:
    - Notes
    - Basic usage example

flags:
    - unstable-api
    - incomplete
---

### Notes

 *  If the decoder finds an ill-formed binary (which, in turn, means that an
    ill-formed instruction was found) then an illegal instruction will be
    generated. Upon invoking such instruction a trap will be signaled to the
    running [`momiji::System`]({{ '/userapi/System' | relative_url }}).

 *  No information about __what__ instruction is being executed is provided,
    other than the decoded string.

 *  The string of the instruction is reconstructed on the fly by the decoder.


### Basic usage example

```cpp
momiji::ExecutableMemory binary = momiji::compile(/* ... */);

// Decode the instruction at offset zero
momiji::DecodedInstruction instr0 = momiji::decode(binary, 0);

// Print the string of the instruction
std::cout << instr0.string << '\n';

// Create an emulated system where the instruction will be executed
momiji::System system;

// Execute the instruction
instr0.exec(system, instr0.data);

// system is now modified
```
