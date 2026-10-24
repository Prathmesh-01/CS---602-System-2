Symbol Table Generator and NM Program

Description:
This project has two C programs. The first program creates a symbol table from an assembly file. The second program reads the symbol table and displays the symbol information.

Files:
- symbol_table.c: Creates the symbol table from an assembly file.
- nm.c: Reads and displays the symbol table.
- input.asm: The assembly input file.
- symbol_table.txt: The output file created by symbol_table.c.

Requirements:
- GCC compiler
- An assembly input file

How to Compile:
1. Compile symbol_table.c:
   gcc symbol_table.c -o create_symbol_table

2. Compile nm.c:
   gcc nm.c -o nm

How to Run:
1. Create the symbol table:
   ./create_symbol_table input.asm

2. Display the symbol information:
   ./nm symbol_table.txt

Symbol Types:
- d: Symbol in the .data section
- b: Symbol in the .bss section
- T: Global symbol
- U: External symbol
- t: Local label in the .text section

Note:
Run the commands in the folder containing the C programs and assembly file. Run nm after symbol_table.txt has been created.
