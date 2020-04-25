---
layout: method
title: getStates
brief: Get all the current system states
overloads:
    '[[nodiscard]] const std::vector<momiji::System>& getStates() const':
        description: "Returns a view of all the System states"
        return: A const-ref to all the System states

flags:
    - deprecated
---

This function is primarily used to query the values of registers and memory during runs.
