---
layout: class
title: momiji::errors::MissingCharacter
in-header: <momiji/Parser.h>
description: |
    Error triggered when an expected character is not found in the parsed
    string.
declaration: struct MissingCharacter

fields:
    'character':
        type: char
        description: The expected character which was not found.
---
