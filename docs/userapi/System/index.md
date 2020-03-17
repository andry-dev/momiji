---
layout: library
title: System
brief: |
    Classes and functions that revolve around the management of an emulated
    MC68000 system (eg: CPU, memory, etc...).



typedefs:
    'momiji::DataRegister':
        definition: |
            <pre><code>Register<std::int32_t, /* implementation-defined */></code></pre>

    'momiji::AddressRegister':
        definition: |
            <pre><code>Register<std::int32_t, /* implementation-defined */></code></pre>

    'momiji::ProgramCounter':
        definition: |
            <pre><code>Register<std::uint32_t, /* implementation-defined */></code></pre>

    'momiji::TrapType':
        definition: |
            <pre><code>std::variant<momiji::traps::InvalidMemoryRead,
                         momiji::traps::InvalidMemoryWrite,
                         momiji::traps::DivisionByZero,
                         momiji::traps::IllegalInstruction>;</code></pre>

---
