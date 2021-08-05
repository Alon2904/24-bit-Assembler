# C Project: 24-bit Assembler
![Build](https://github.com/avivnaaman/20465-project/workflows/Build/badge.svg?branch=master)
## About
This project is an assignment in a C programming course at the open unvieristy (20465). The purpose of this project is to build an assembler for 15-instruction asssembly language, for an imaginary 24-bit CPU and 12 bits of memory size.
This project was done by me.

Project instructions in the Hebrew language are added and named `instructions.pdf`.

For any issues, questions and ideas turn to the Issues tab.

License: MIT License, Affect from May 1st, 2021
## Source structure (Modules)
1. `assembler` - main function definition, argv & argc processing, single file processing.
2. `first_pass` - contains functions for processing a single line in the first pass and a code line in the first pass.
3. `second_pass` - contains function for processing a single line in second pass and replacing symbol by it's address.
4. `textAnalyze` - contains functions for proccesing strings.
5. `base` - contains necessary data in order to proccess the input.
6. `lists` - contains functions to handle linked lists.
11. `writefiles` - contains methods for writing output files: `*.ob`, `*.ext` and `*.ent` 

## Usage
`./assembler file1 file2 file3 ...`
Will start the processing of each file (specify WITHOUT extension!). No support for double-quoting file names.

## Build
You may build with several tools:
1. Using CMake: run `cmake --build .` (you might have to run `cmake --cofngiure .` first) - Provides support for CLion IDE
2. Using `make` command (Makefile)
3. Manually (Not recommended): compile each `*.c` file to `*.o` and then compile all to a single executable using `CC`/`GCC`/`CLang` with `-g a.o b.o .... -o exe`
