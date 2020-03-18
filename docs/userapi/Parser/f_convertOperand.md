---
layout: function
title: momiji::convertOperand
in-header: <momiji/Parser.h>
brief: |
    Converts a [`momiji::Operand`]({{ '/userapi/Parser#typedefs' | relative_url }}) to a
    [`momiji::ParserOperand`]({{ '/userapi/Parser/e_ParserOperand' | relative_url }})

overloads:
    'momiji::ParserOperand convertOperand(const momiji::Operand& operand)':
        arguments:
            - name: operand
              type: 'const momiji::Operand&'
              description: The Operand to convert
        return: The ParserOperand which corresponds to the type of the Operand

flags:
    - bad-name
---
