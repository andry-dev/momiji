---
layout: method
title: newState
brief: Creates a new state
overloads:
    "std::optional<momiji::ParserError> newState(const std::string& str)":
        arguments:
            - description: The string to parse
              name: str
              type: const std::string&
        description: |
            Tries to parse the string and, if successful, creates a new System state
        return: 'An optional containing the parsing error, else an empty optional'


    "void newState(momiji::ExecutableMemory binary)":
        arguments:
            - name: binary
              type: ExecutableMemory
              description: The raw executable memory to read
        description: |
            Reads a raw, compiled, program and creates a new system state with the executable memory set to that of the provided binary.
            No checking is done for the correctness of the binary
---
