---
layout: function
title: momiji::decode
in-header: <momiji/Decoder.h>
description: |
    Decodes one instruction at a specific offset inside an executable memory.

overloads:
    'momiji::DecodedInstruction decode(momiji::ConstExecutableMemoryView mem, std::int64_t idx)':
        arguments:
            - name: mem
              type: momiji::ConstExecutableMemoryView
              description: A view over the memory of the executable.

            - name: idx
              type: std::int64_t idx
              description: |
                Offset to where the decoder should read an instruction.
---
