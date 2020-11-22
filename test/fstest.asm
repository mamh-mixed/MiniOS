[bits 32]

; 这个文件按照顺序做了下列事情：
;   * 打开文件 README.txt
;   * 打开文件 stdout
;   * 读取文件 README.txt
;   * 将读出出的内容写入 stdout

section .text vstart=0x1000_0000
    jmp start
    start:

        push dword 512
        push dword 0
        push dword 1
        push dword file_README
        call open
        add esp,16

        push dword 512
        push dword 0
        push dword 0
        push dword file_stdout
        call open
        add esp,16

        push dword buffer0
        push dword file_README
        call read
        add esp,8

        push dword 512
        push dword buffer0
        push dword file_stdout
        call write
        add esp,12

        jmp $

    
    open:
        push ebp
        mov ebp,esp
        push ecx

        mov eax,1
        mov ecx,4
        push dword [ebp+4*5]
        push dword [ebp+4*4]
        push dword [ebp+4*3]
        push dword [ebp+4*2]
        int 0x80
        add esp,16

        pop ecx
        pop ebp
        ret
    
    read:
        push ebp
        mov ebp,esp
        push ecx

        mov eax,4
        mov ecx,2
        push dword [ebp+4*3]
        push dword [ebp+4*2]
        int 0x80
        add esp,8

        pop ecx
        pop ebp
        ret

    write:
        push ebp
        mov ebp,esp
        push ecx

        mov eax,5
        mov ecx,3
        push dword [ebp+4*4]
        push dword [ebp+4*3]
        push dword [ebp+4*2]
        int 0x80
        add esp,12

        pop ecx
        pop ebp
        ret


file_stdout db "stdout",0x00
file_README db "README.txt",0x00

buffer0 times 4096 db 0
buffer1 times 4096 db 0
    
msg db "No cost too great.",0x0a
    db "No mind to think.",0x0a
    db "No will to break.",0x0a
    db "No voice to cry suffering.",0x0a
    db "Born of God and Void.",0x0a
    db "You shall seal the blinding light that plagues their dreams.",0x0a
    db "You are the Vessel.",0x0a
    db "You are the Hollow Knight.",0x0a,0x00

; times 510-($-$$) db 0
;                  db 0x55,0xaa
