%macro lp 2
    mov eax,%1
    mov ebx,%2
    cmp eax,ebx
    jz end
%%end
    end:
%endmacro

%macro Myadd 3
    mov eax,%1
    mov ebx,%2
    add eax,ebx
    mov %3,eax
%endmacro
%macro Myadd 2
    mov eax,%1
    mov ebx,%2
    add eax,ebx
%endmacro

section .text
    global main
    extern printf
main:
    mov eax,10
    mov ebx,20
    Myadd eax,ebx,ecx
    Myadd eax,ebx
    lp eax,ebx
    ret

