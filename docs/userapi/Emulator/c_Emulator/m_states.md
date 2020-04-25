---
layout: method
title: states
brief: Obtain a span of the current system states
overloads:
    '[[nodiscard]] span<momiji::System> states() noexcept':
        description: "Returns a span of all the System states"
        return: A span to all the System states
---

This function is primarily used to query the values of registers and memory
during emulator runs.
