section .text

    global _asm_block_or_suspend_process

    extern _asm_int0x20_entry

    _asm_block_or_suspend_process:
        pushf
        and dword [esp],0xffff_fdff
        push cs
        call _asm_int0x20_entry
        ret
