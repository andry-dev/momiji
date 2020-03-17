---
layout: function
title: 'momiji::make_memory_view'
brief: |
    Creates a [view of the memory](/userapi/Memory) of a running [`momiji::System`](/userapi/System/System) instance.
in-header: '<momiji/System.h>'
description: |
    Creates a view of the memory of a running System instance.

overloads:
    'inline momiji::ExecutableMemoryView make_memory_view(momiji::System& sys)':
        description: |
            Creates a span of memory from a modifiable system.
            This is useful to modify memory that does not need to expand itself.
        return: A modifiable view of the memory of the system.
        arguments:
            - name: sys
              type: 'momiji::System&'
              description: The <em>modifiable</em> system instance.

    'inline momiji::ConstExecutableMemoryView make_memory_view(const momiji::System& sys)':
        description: |
            Creates a view of the memory from an immutable system.
            This is useful to obtain a read-only view of memory.
        return: A read-only view of the memory of the system.
        arguments:
            - name: sys
              type: 'const momiji::System&'
              description: The <em>immutable</em> system instance.
---
