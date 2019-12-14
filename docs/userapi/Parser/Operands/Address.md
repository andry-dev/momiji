---
layout: class
title: momiji::operands::Address
description: |
    An operand in the form `(a*)` where '\*' is any number from 0 to 7.
    This references the address inside an address register.
fields:
    reg:
        type: std::int32_t
        description: 'The number of the address register. (eg: 0 is the first, 7 is the eight)'
---
