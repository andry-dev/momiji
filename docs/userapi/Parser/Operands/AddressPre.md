---
layout: class
title: momiji::operands::AddressPre
in-header: <momiji/Parser.h>
declaration: struct AddressPre
description: |
    An operand in the form `(a*)+` where '\*' is any number from 0 to 7.
    This references the address inside an address register.
    When used, the value in the address register is decremented by the data size
    of the instruction and then returned.
    fields:
    reg:
        type: std::int32_t
        description: 'The number of the address register. (eg: 0 is the first, 7 is the eight)'
---
