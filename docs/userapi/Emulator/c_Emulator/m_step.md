---
layout: method
title: step
brief: Executes the next instruction
overloads:
    'bool step()':
        return: true if it was possible to execute the instruction, false otherwise
---

### Remarks

The emulator does bound checking on the executable's memory, preventing it from loading an invalid instruction (eg: an out of bounds one).
That's an example where `step()` would return `false`.
