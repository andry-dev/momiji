---
layout: class
title: momiji::errors::UnexpectedCharacter
in-header: <momiji/Parser.h>
description: |
    Error triggered when an invalid character is found during parsing.
declaration: struct UnexpectedCharacter

fields:
    'character':
        type: char
        description: The character found.
---
