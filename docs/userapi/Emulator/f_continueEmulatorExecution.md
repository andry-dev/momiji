---
layout: function
title: momiji::continueEmulatorExecution
brief: Continues the execution of an emulator until possible
in-header: "<momiji/Emulator.h>"
overloads:
    'void continueEmulatorExecution(Emulator& emu) noexcept':
        description: "Steps through the emulator until there are no more instructions to execute or an error occurs"
        arguments:
            - type: Emulator&
              name: emu
              description: The emulator of which the execution shall continue
    'void continueEmulatorExecution(Emulator& emu, F&& fun) noexcept':
        description: |
            Steps through the emulator until there are no more instructions to execute or an error occurs.<br/>
            For each instruction executed, the functor F is called
        arguments:
            - type: Emulator&
              name: emu
              description: The emulator of which the execution shall continue
            - type: F&&
              name: fun
              description: |
                The functor to be called for every instruction executed.
                The functor shall be of the form <code>void(void)</code>


---
This function is equivalent to doing the following:

```cpp
while (emu.step())
{
}
```

Or the following:

```cpp
while (emu.step())
{
    fun();
}
```
