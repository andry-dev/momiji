---
layout: class
title: momiji::operands::AddressPost
in-header: <momiji/Parser.h>
declaration: struct AddressPost
description: |
    An operand in the form `(a*)+` where '\*' is any number from 0 to 7.
    This references the address inside an address register. When used, a copy of
    the value inside the address register is made and the value in the registrer
    is incremented by the data size of the instruction.
    The returned value is the copy before the increment.
fields:
    reg:
        type: std::int32_t
        description: 'The number of the address register. (eg: 0 is the first, 7 is the eight)'
---
