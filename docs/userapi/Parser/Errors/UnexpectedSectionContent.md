---
layout: class
title: momiji::errors::UnexpectedSectionContent
in-header: <momiji/Parser.h>
description: |
    Error triggered when there's code in a data section or viceversa.
declaration: struct UnexpectedSectionContent

fields:
    'section':
        type: momiji::ParserSection
        description: The section found.
---
