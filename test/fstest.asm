[bits 32]

section .text vstart=0x1000_0000
    jmp start
    start:
        jmp $

times 510-($-$$) db 0
                 db 0x55,0xaa
