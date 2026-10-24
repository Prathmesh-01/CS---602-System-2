README
======

Project: Assembly File Validator

1. About
--------
This is a C program that checks an assembly language file.
It checks mnemonics, registers, and assembly sections.
It also displays hexadecimal output for data and supported instructions.

2. Required Files
-----------------
program.c   - C source code
modrm.txt   - Instruction table
input.asm   - Assembly file to check

3. Requirements
---------------
GCC compiler
modrm.txt file
Assembly input file

4. Compile
----------
gcc program.c -o validate

5. Run
------
./validate input.asm

Replace input.asm with your assembly file name.

6. Important
------------
Keep modrm.txt in the same directory where you run the program.

The program supports only the instructions implemented in the code.
Not all assembly instructions may be supported.
