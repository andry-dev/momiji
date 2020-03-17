---
layout: method
title: as<To>
in-header: <momiji/System.h>
description: |
    Reinterpret casts the address of the stored value to an arbitrary
    pointer of type "To".

overloads:
    '[[nodiscard]] To* as()':
        return: |
            A pointer to the address of the stored value, reinterpret to the
            type "To".
        template-arguments:
            - name: To
              description: The raw type to cast to.
---

**THIS FUNCTIONS IS UNSAFE, USE WITH CARE.**
