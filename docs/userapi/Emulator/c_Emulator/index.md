---
layout: class
title: momiji::Emulator
in-header: "<momiji/Emulator.h>"
declaration: "struct Emulator"
brief: A class that manages System states
---

A "System state" is a particular value in time of a [`momiji::System`]({{ '/userapi/System' | relative_url }}).
This class simply manages a collection of `System`s to allow rollback.

By default a new state is created when `step()` is invoked, this can be changed
by tweaking [`EmulatorSettings::RetainStates`]({{ '/userapi/Emulator/c_EmulatorSettings' | relative_url }}). Creating a new state implies copying
all the data from the previous `System` to the new one and executing the next
instruction, this is unavoidable to implement non-destructive writes.

### Future

In the future it may make sense to employ a persistent data structure to keep
track of the states, maybe by making `System` COW.
