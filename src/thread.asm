

section .text
    global _asm_create_thread_dpl0

    _asm_create_thread_dpl0:
    ; extern Uint32 _asm_create_thread_dpl0(Uint32 cs, Uint32 ds, Uint32 es, Uint32 fs, Uint32 gs, Uint32 esp0, Uint32 eip);
        
        push ebp
        mov ebp,esp
        push ebx

        mov eax,esp
        mov esp,[ebp+0x04*7]
        pushf
        or dword [esp],0x200
        push dword [ebp+0x04*2]
        push dword [ebp+0x04*8]
        push dword [ebp+0x04*3]
        push dword [ebp+0x04*4]
        push dword [ebp+0x04*5]
        push dword [ebp+0x04*6]
        pushad
        mov ebx,esp
        mov esp,eax
        mov eax,ebx
        pop ebx
        pop ebp
        ret

    global _asm_create_thread_dpl3

    _asm_create_thread_dpl3:
    ; extern Uint32 _asm_create_thread_dpl3
            ; (Uint32 cs, Uint32 ss3, Uint32 ds, Uint32 es, Uint32 fs, Uint32 gs, Uint32 esp0, Uint32 esp3, Uint32 eip);

        push ebp
        mov ebp,esp
        push ebx

        mov eax,esp
        mov esp,[ebp+0x04*8]

        push dword [ebp+0x04*3]
        push dword [ebp+0x04*9]
        pushf
        or dword [esp],0x200
        push dword [ebp+0x04*2]
        push dword [ebp+0x04*10]
        push dword [ebp+0x04*4]
        push dword [ebp+0x04*5]
        push dword [ebp+0x04*6]
        push dword [ebp+0x04*7]
        pushad

        mov ebx,esp
        mov esp,eax
        mov eax,ebx

        pop ebx
        pop ebp
        ret

    
    global _asm_test

    _asm_test:
        mov eax,1
        mov ebx,2
        mov ecx,3
        mov edx,4
        mov edi,5
        mov esi,6
        push 1
        push 2
        push 3
        push 4
        push 5
        push 6
        jmp $
        ret


