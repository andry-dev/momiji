# momiji

A basic Motorola 68000 emulator infrastructure written in C++17.
This is not production-ready and I will only ever implement a subset of the assembler.

"momiji" itself refers to the entire project, from the library (libmomiji), the GUIs (momiji-gl and momiji-qt) to the CLI tools (momiji-as, momiji-dump, momiji-diff, etc...).

## Motivation

This was created out of frustration from using EASy68k.
I wanted something that could be used to make tools and better GUIs, something that supported backtracking, diffs and other stuff.
So I started working on this, initially as a REPL, then it evolved in a compiler and disassembler.

## libmomiji

The core of the project. It includes a parser (`<momiji/Parser.h>`), a compiler (`<momiji/Compiler.h>`), a decoder/disassembler (`<momiji/Decoder.h>`) and finally an emulator class (`<momiji/Emulator.h>`) to make everything work together effortlessly.

## momiji-tools

Brief description of the provided CLI tools:

| Name             | Description                               |
|------------------|-------------------------------------------|
| `momiji-as`      | A basic compiler                          |
| `momiji-dump`    | Yields a compiled program's trace         |
| `momiji-diff`    | Creates a diff of two programs executions |

Keep in mind that, at the time of writing, they are incomplete and __really__ basic.

## momiji-gl

Main GUI, using ImGui and [tewi](https://github.com/andry-dev/tewi).
This is where most features will be implemented.
**This doesn't work on OSX.**

## momiji-qt

A Qt GUI, provided for the people who can't run `momiji-gl` (like OSX users).
At the time of writing, this is mostly under-developed.

# Building

## `libmomiji`

You need [asl](https://github.com/andry-dev/asl) to build `libmomiji`.
Just create a conan package for that and then:

```
mkdir build
cd build
conan install ..
cmake ..
```

And you're done with just the core.

## `momiji-gl`

You also need [tewi](https://github.com/andry-dev/tewi), you can either use the local submodule (`git submdule update --init`) or create the conan package.

Then just pass `-DMOMIJI_BUILD_GL_GUI=1` to CMake:

```
cd build
cmake .. -DMOMIJI_BUILD_GL_GUI=1
```

## `momij-qt`

You obviously need Qt5.

```
cd build
cmake .. -DMOMIJI_BUILD_QT_GUI=1
```

## momiji-tools

Since these are simple CLI tools you don't need anything special (apart from `libmomiji`, obviously).

```
cd build
cmake .. -DMOMIJI_BUILD_TOOLS=1
```


Obviously, you can build all of them at once if you feel like.
