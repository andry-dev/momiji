---
layout: enum
title: momiji::ParserSection
in-header: "<momiji/Parser.h>"
declaration: 'enum class ParserSection : std::int8_t'
brief: A _lightweight_ enum describing a code section encountered during parsing.
description: This enum is used for LIGHTWEIGHT error checking. The parser uses this enum when checking if there is data in a code section or viceversa.
values:
    - name: Code
    - name: Data
---
