BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Włącz A20
    in al, 0x92
    or al, 9
    out 0x92, al

    ; Załaduj kernel (1 sektor po bootloaderze, do 0x1000)
    mov bx, 0x1000           ; docelowy adres
    mov dh, 1                ; ilość sektorów (zmień jeśli kernel > 512B)
    call load_kernel

    ; Przełącz do protected mode
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:pm_start

; ---------------------------------
gdt_start:
    dq 0x0000000000000000 ; Null
    dq 0x00cf9a000000ffff ; Code segment
    dq 0x00cf92000000ffff ; Data segment
gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ---------------------------------
[bits 32]
pm_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x1000              ; skok do kernela

; ---------------------------------
[bits 16]
load_kernel:
    pusha
    mov ah, 0x02    ; BIOS: read sectors
    ; AL już ustawiony na ilość sektorów do wczytania
    mov ch, 0x00
    mov cl, 0x02    ; sektor 2 (kernel po MBR)
    mov dh, 0x00    ; głowica 0
    mov dl, 0x00    ; dysk 0
    mov es, bx
    mov bx, 0x0000
    int 0x13
    jc disk_read_error
    popa
    ret

disk_read_error:
    mov si, disk_error_msg
.print:
    lodsb
    or al, al
    jz .hang
    mov ah, 0x0e
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp .print
.hang:
    cli
    hlt
    jmp .hang

disk_error_msg db "Disk Error!",0

times 510-($-$$) db 0
dw 0xAA55