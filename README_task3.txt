README
======

Project: Assembly File Validator

1. About
--------
This is a C program that checks an assembly language file.

It checks:
- Assembly sections such as .data, .bss, and .text
- Variable declarations
- Mnemonics using modrm.txt
- Register names
- Hexadecimal output for data

2. Required Files
-----------------
- 10_09_2026.c : C source code
- modrm.txt    : Instruction table
- your_file.asm: Assembly file to check

3. Requirements
---------------
- GCC compiler
- modrm.txt file
- Assembly input file

4. Compile
----------
Open the terminal in the project folder and run:

gcc 10_09_2026.c -o validate

5. Run
------
Use the following command:

./validate your_file.asm

Replace your_file.asm with your assembly file name.

6. Important
------------
Keep modrm.txt in the same directory where you run the program.

The program checks only the instructions and formats implemented in the source code.
It may not support every assembly instruction.