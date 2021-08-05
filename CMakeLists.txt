cmake_minimum_required(VERSION 3.15.3)
project(assemblerProject C)

set(CMAKE_C_STANDARD 99)

add_executable(assemblerProject  first_pass.c base.h assembler.c first_pass.h lists.c lists.h textAnalyze.c textAnalyze.h symbolTable.c symbolTable.h second_pass.c second_pass.h writeFiles.c writeFiles.h assembler.h )