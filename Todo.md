- [X] The parser should be restructured, again
- [X] The parser should accept math expressions and operations on labels
- [ ] I should've wrote tests
- [ ] There is no concept of sections right now, in fact the emulator executes
      anything, even raw memory
      - Insert a jump to the correct program start?
      - Add some initial binary information?
      - EASy68k doesn't store any of that information in the binary, it just
        knows where to jump; but here there's an actual compiler and
        interpreter, the latter can't know about this stuff
