---
layout: function
title: momiji::convertOperand
in-header: <momiji/Parser.h>
brief: |
    Converts a [`momiji::Operand`](/userapi/Parser#typedefs) to a
    [`momiji::ParserOperand`](/userapi/Parser/e_ParserOperand)

overloads:
    'momiji::ParserOperand convertOperand(const momiji::Operand& operand)':
        arguments:
            - name: operand
              type: 'const momiji::Operand&'
              description: The Operand to convert
        return: The ParserOperand which corresponds to the type of the Operand
---
