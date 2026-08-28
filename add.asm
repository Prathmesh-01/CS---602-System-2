section .data
    string db "sairaj",10,0
    op db "output %d",10,0
section .bss
    a resb 1
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
