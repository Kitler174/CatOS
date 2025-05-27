section .text
    bits 64

    global _start
    extern efi_main

_start:
    ; UEFI w trybie x86-64 startuje w long mode, więc nie trzeba przechodzić w legacy mode
    mov rdi, rdx         ; RDX zawiera adres EFI System Table
    call efi_main        ; Wywołanie funkcji efi_main

    ; Jeśli jądro zwróciło, zakończ Boot Services
    mov rdi, 0           ; Argument: kod zakończenia (0)
    mov rax, 0x18        ; EFI Boot Services Exit
    call [rdx + 24]      ; Zakończenie Boot Services

    ; Zatrzymanie procesora
    cli
    hlt

section .bss
    align 16
    stack_bottom:
        resb 4096 * 16   ; 64 KB stosu
    stack_top:
