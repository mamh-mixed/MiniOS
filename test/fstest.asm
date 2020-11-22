[bits 32]

section .text vstart=0x1000_0000
    jmp start
    start:
        mov eax,1
        mov ecx,1
        push file_stdout
        int 0x80
        add esp,4

        mov eax,1
        mov ecx,1
        push file_README
        int 0x80
        add esp,4

        mov eax,4
        mov ecx,2
        push buffer0
        push file_README
        int 0x80
        add esp,8

        mov eax,5
        mov ecx,3
        push dword 512
        push buffer0
        push file_stdout
        int 0x80
        add esp,12

        mov eax,7
        mov ecx,0
        int 0x80

        mov eax,5
        mov ecx,3
        push dword 512
        push buffer0
        push file_stdout
        int 0x80
        add esp,12

        jmp $


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
