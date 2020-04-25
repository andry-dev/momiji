---
layout: method
title: constStates
brief: Obtain a view of the current system states
overloads:
    '[[nodiscard]] span<const momiji::System> constStates() const noexcept':
        description: "Returns a view of all the System states"
        return: A const span to all the System states
---

This function is primarily used to query the values of registers and memory
during emulator runs.
