---
layout: method
title: reset
brief: Resets the emulator to the initial state
overloads:
    'bool reset()':
        return: true if it there was any non-initial System state before attempting the reset, false otherwise
---

### Remarks

Calling `reset()` on an empty Emulator has no effect, other than getting `false` returned from the function.
