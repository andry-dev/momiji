---
layout: class
title: momiji::Register<IntType, PhantomTag>
description: Generic class describing a register with its value.

in-header: <momiji/System.h>
declaration: |
    template < typename IntType, typename PhantomTag >
    struct Register

typedefs:
    'value_type':
        definition: IntType
        description: |
            A typedef of the first template parameter, for usage in
            generic code.
---
