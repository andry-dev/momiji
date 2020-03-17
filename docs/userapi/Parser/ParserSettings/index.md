---
layout: class
in-header: "<momiji/Parser.h>"
title: momiji::ParserSettings
brief: Settings for the parser
description: A struct representing settings for the parser
declaration: struct ParserSettings
fields:
    breakpoints:
        type: "`gsl::span<Breakpoint>`"
        description: A span of breakpoints to be passed to the Parser
---

A `gsl::span` is a view over contiguous memory, it expects the memory to be allocated elsewhere. It essentially wraps a pointer + size.
A `gsl::span` doesn't cleanup the resources.

### Example

```cpp
// Creates two breakpoints
std::array<momiji::Breakpoint, 2> breakpoints = {
    { 10 }, // line 10
    { 2 }, // line 2
};

ParserSettings settings;
// Initializes the span
settings.breakpoints = { breakpoints.data(), breakpoints.size() };

auto res = momiji::parse("...", settings);
```

Using a vector:

```cpp
// Creates two breakpoints
std::vector<momiji::Breakpoint> breakpoints = {
    { 10 }, // line 10
    { 2 }, // line 2
};

ParserSettings settings;
// Initializes the span
settings.breakpoints = { breakpoints.data(), breakpoints.size() };

auto res = momiji::parse("...", settings);
```

### Motivation

I didn't want to force a specific type for holding a simple view of the
breakpoints.
Using directly a `std::vector` would imply that one could only use the default
allocator and only allocate dynamically.
A pointer + size (span) is a better approach for just a view.
