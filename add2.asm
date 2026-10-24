section .data
    string db "sairaj",10,0
    op db "output %d",10,0
    as db "prathmesh abhishek^2 sairaj",10,0
    Array dd 90,22,39
    a dd 19,20,30,201,20,30392,1029292
section .bss
    b resb 1
section .text
    global main
    extern printf
main:
    mov eax,10
    mov ecx,20
    add eax,ecx
    push eax
    push op
    call printf
    add ebp,8
    ret
