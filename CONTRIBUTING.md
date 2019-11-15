# On Contributing guidelines

## Code of Conduct

I don't really care who you are and what you do in your life; for me everything revolves around having __respect__ for other individuals (when reasonable, else see below). This means that you should feel free to contribute but keep in mind the following rules:

 1. Don't be an asshole.
    - Expect to get told if you are being one.
    - Expect consequences if you don't stop being one.
 2. I don't want racism and/or bigotry in my repos. Please respect other people.
    - Don't use derogatory terms and/or offensive language.
    - Don't troll.
    - Don't insult other people.
    - Don't do harm to (and/or harass) others.
 3. Accept constructive criticism.

Failure to accept the previous rules _will_ result in a project-wide ban --- that's absolute.
You're an adult, not a child: you should know better.

## How should I submit a bug report?

* __Please specify WHAT you're doing.__ Like, if you're using the library to write custom tooling or if you're just using one of the bundled programs.
* __Please specify WHAT PART of the project doesn't work__, doing so would really help when individuating the causes of the bugs.
  This means specifying what program you're using (if any), what function you're calling (if writing a custom tool), etc...
* __Please specify what behaviour you expected.__
* __Please attach images when appropriate.__

## How should I submit a proposal?

Open an issue with the `[RFC]` tag in the title, eg:

> [RFC] Add a debugger CLI tool

In such issue, specify what exactly you want, why you want it and possible ideas on implementing it.

## Code style

 * Run `clang-format` (`./format.sh` or integrate it in your editor) before committing.
 * For new code, please try to respect clang-tidy checks if reasonable.
 * I don't explicitly support compilers older than GCC 9.2, Clang 9 and MSVC 2019. This means that code should be using the latest enhancements available and not bend backwards to support older systems.
 * Code should work on all platforms by default, exceptions are on particular compiler configurations (for example, there's no `<filesystem>` support on current Mac OS X, so some tools are disabled on OS X).
   A PR will not be merged unless it passes all the checks.

## Commits

 * Don't add too much changes to a single commit.
 * Avoid doing commits like "revert changes" or "fix typo", during a PR just amend and force-push.

Commits should be structured like so:

```
Component X: Brief description of the main changes

Component Y: Brief description of minor changes
Component Z: Brief description of minor changes

For non-trivial commits add a wall of text describing why the commit is useful/needed and what it changes in particular.
```

The first line is the commit title, that should be the most important change you did.
__Please don't continue the commit on the second line!__ The second line should always be empty.

The "Component X/Y/Z" can be one of the following:


| Name           | Description |
|----------------|-------------|
| `All`          | Affects everything in the project, stuff like new dependencies, different project layout, etc... |
| `CI`           | Affects the CI infrastructure with GitHub Actions, like adding or modifying workflows. |
| `Core`         | Affects core _general_ library changes. |
| `CMake`        | Major `CMakeLists.txt` changes. |
| `New tool`     | Used when a new tool is made, under `momiji-tools` |
| `Instructions` | Affects changes to the implementation of the instructions (also used when new instructions are implemented). |
| `Todo`         | Add or remove items in `Todo.md` |
| `Compiler`     | Affects anything in `libmomiji/include/momiji/Compiler.h` and `libmomiji/src/Compiler` |
| `Decoder`      | Affects anything in `libmomiji/include/momiji/Decoder.h` and `libmomiji/src/Decoder` |
| `Emulator`     | Affects anything in `libmomiji/include/momiji/Emulator.h` and `libmomiji/src/Emulator.cpp` |
| `Memory`       | Affects anything in `libmomiji/include/momiji/Memory.h` |
| `Parser`       | Affects anything in `libmomiji/include/momiji/Parser.h` and `libmomiji/src/Parser` |
| `System`       | Affects anything in `libmomiji/include/momiji/System.h` |
| `momiji-as`    | Affects anything in `momiji-tools/src/assembler.cpp` |
| `momiji-diff`  | Affects anything in `momiji-tools/src/diff.cpp` |
| `momiji-dump`  | Affects anything in `momiji-tools/src/dump.cpp` |
| `momiji-gl`    | Affects anything in `momiji-gl` |
| `momiji-qt`    | Affects anything in `momiji-qt` |

## Documentation

Todo. Documentation is still missing unfortunately.

## Translations for momiji-qt

Help wanted for translating momiji-qt in other languages.
Right now Italian is the sole other secondary language.

To help with translations please use Qt5 Linguist.
