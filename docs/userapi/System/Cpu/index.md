---
layout: class
title: momiji::Cpu
brief: Class describing a generic MC68000 CPU.
in-header: '<momiji/System.h>'
declaration: struct Cpu
description: Class describing a generic MC68000 CPU.

fields:
    dataRegisters:
        type: 'std::array<momiji::DataRegister, 8>'
        description: |
            Array of eight data registers indexed from zero (`d0`) to seven
            (`d7`).
        default: All zeros.

    addressRegisters:
        type: 'std::array<momiji::AddressRegister, 8>'
        description: |
            Array of eight address registers indexed from zero (`a0`) to seven
            (`a7`). The last one (`a7`) is the stack pointer.
        default: All zeros.

    statusRegister:
        type: 'momiji::StatusRegister'
        description: The status register.
        default: All flags cleared out.

    programCounter:
        type: 'momiji::ProgramCounter'
        description: The current program counter.
        default: 0

toc: Notes
---

### Notes

The emulated CPU has no concept of a "clock", each instruction is executed
"instantaneously", as if it took 1 clock to execute.
