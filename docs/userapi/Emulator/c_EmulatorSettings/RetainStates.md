---
layout: enum
title: momiji::RetainStates
in-header: "<momiji/Emulator.h>"
description: |
    Policy to determine if the emulator should maintain previous system states 
    when stepping through the assembled code.
brief: |
    Policy to determine if the emulator should maintain previous system states 
    when stepping through the assembled code.
declaration: "enum class RetainStates : std::int8_t"
values:
    - name: Always
      description: |
        Tells the emulator to always create new copies of the system states
        for each instruction execution.
    - name: Never
      description: |
        Tells the emulator to never keep track of the various system states.
        In this mode, each instruction modifies the same
        [`momiji::System`]({{ '/userapi/System' | relative_url }}), doing a
        [`rollback()`]({{ '/userapi/Emulator/c_Emulator/m_rollback' | relative_url }}) is equivalent to resetting
        the emulator ([`reset()`]({{ '/userapi/Emulator/c_Emulator/m_reset)' | relative_url }}).

        Parsing a new source code in this mode will also reset the emulator.
---
