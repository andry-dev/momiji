---
layout: class
title: momiji::System
description: |
    Class that represents a running MC68000 system.
in-header: '<momiji/System.h>'
declaration: struct System

fields:
    cpu:
        type: 'momiji::Cpu'
        description: |
            The emulated CPU with all its registers.

    mem:
        type: 'momiji::ExecutableMemory'
        description: |
            The memory of the running executable and stack space.

    trap:
        type: 'std::optional<momiji::TrapType>'
        description: |
            If non-empty, it contains the last trap that was generated.
        default: Empty --- no trap is initially generated.
---
