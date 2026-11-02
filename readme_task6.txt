# C Macro Preprocessor

## 1. Introduction

This project implements a simple macro preprocessor in C for assembly language files.

It reads an assembly file, identifies macro definitions, stores the macro instructions, and expands macro calls by replacing parameters with the actual arguments.

## 2. Features

- Reads an assembly file using a command-line argument.
- Identifies macro definitions using `%macro`.
- Identifies the end of a macro using `%endmacro`.
- Stores macro instructions in an array.
- Supports macro parameters such as `%1`, `%2`, `%3`, etc.
- Replaces macro parameters with actual arguments.
- Prints the expanded assembly code.
- Uses custom string functions instead of `string.h`.
- Uses only the `stdio.h` header file.

## 3. Requirements

- GCC compiler
- Linux or any system supporting C compilation

## 4. Compilation

Compile the program using:

```bash
gcc macro.c -o getmacro
```

## 5. Execution

Run the program by providing the assembly filename as a command-line argument:

```bash
./getmacro input.asm
```

## 6. Input Example

Create a file named `input.asm`:

```asm
%macro Myadd 3
    mov eax,%1
    mov ebx,%2
    add eax,ebx
    mov %3,eax
%endmacro

section .text
global main

main:
    mov eax,10
    mov ebx,20
    Myadd eax,ebx,ecx
    ret
```

## 7. Expected Output

```asm
section .text
global main

main:
    mov eax,10
    mov ebx,20
    mov eax,eax
    mov ebx,ebx
    add eax,ebx
    mov ecx,eax
    ret
```

## 8. Working

The program works in the following steps:

1. Opens the assembly file.
2. Reads the file line by line.
3. Searches for the `%macro` keyword.
4. Reads the macro name and number of parameters.
5. Stores all instructions until `%endmacro` is found.
6. Checks each remaining line for a macro invocation.
7. Reads the actual arguments passed to the macro.
8. Replaces `%1`, `%2`, `%3`, etc., with the corresponding arguments.
9. Prints the expanded assembly code.

## 9. Limitations

- Supports only one macro definition.
- Supports a maximum of 10 macro arguments.
- Supports a maximum of 50 macro body lines.
- Each macro body line can contain up to 255 characters.
- Does not support nested macros.
- Does not support recursive macro expansion.

## 10. Header File

Only the following header file is used:

```c
#include <stdio.h>
```

String operations such as string length, comparison, and copying are implemented using custom functions.
