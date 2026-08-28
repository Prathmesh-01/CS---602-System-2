- First of all make a executable by following command :
gcc checks_valid_mnemonics_register.c -o checks

- After the creation of the executable "checks" 
- To run the checks executable 
./checks add.asm

- The above command ./checks add.asm checks all the mnemonics in the file "add.asm" and compares it with the "modrm.txt" 
- If the mnemonics exist then no error if not then it returns a error that mnemonics doesn't exists
- It also checks all the registers i.e
eax, ebx, ecx, edx, esp, ebp, esi, edi
- If the registers is from the above sequece then it doesn't return any error 
- If the registers are not from the above sequence then it return error
