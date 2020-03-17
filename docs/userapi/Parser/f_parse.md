---
layout: function
title: momiji::parse
in-header: "<momiji/Parser.h>"
brief: Parses an m68k source code
overloads:
    'momiji::ParsingResult parse(const std::string& str)':
        arguments:
            - type: 'const std::string&'
              name: str
              description: The source code to parse
        return: A result type representing either the parsed code or an error

    'momiji::ParsingResult parse(const std::string& str, ParserSettings settings)':
        arguments:
            - type: 'const std::string&'
              name: str
              description: The source code to parse
            - type: ParserSettings
              name: settings
              description: The settings for the parser
        return: A result type representing either the parsed code or an error

flags:
    - unstable-abi
---
