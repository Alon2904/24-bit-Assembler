# Basic compilation macros
CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
GLOBAL_DEPS = assembler.h # Dependencies for everything
EXE_DEPS = assembler.o first_pass.o lists.o second_pass.o symbolTable.o textAnalyze.o writeFiles.o # Deps for exe

## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

## Main:
assembler.o: assembler.c assembler.h $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

## Code helper functions:
lists.o: lists.c lists.h $(GLOBAL_DEPS)
	$(CC) -c lists.c $(CFLAGS) -o $@

## First Pass:
first_pass.o: first_pass.c first_pass.h $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

## Second Pass:
second_pass.o: second_pass.c second_pass.h $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

## Instructions helper functions:
symbolTable.o: symbolTable.c symbolTable.h $(GLOBAL_DEPS)
	$(CC) -c symbolTable.c $(CFLAGS) -o $@

## Table:
textAnalyze.o: textAnalyze.c textAnalyze.h $(GLOBAL_DEPS)
	$(CC) -c textAnalyze.c $(CFLAGS) -o $@





## Output Files:
writefiles.o: writefiles.c writefiles.h $(GLOBAL_DEPS)
	$(CC) -c writefiles.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -rf *.o
