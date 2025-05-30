; void context_switch_asm(uint32_t* old_regs, uint32_t* new_regs, uint32_t* old_eip, uint32_t new_eip);

global context_switch_asm
section .text
context_switch_asm:
    ; old_regs: [esp+4]
    ; new_regs: [esp+8]
    ; old_eip:  [esp+12]
    ; new_eip:  [esp+16]

    ; Save current registers to old_regs
    mov  eax, [esp+4]
    mov  [eax+0], edi
    mov  [eax+4], esi
    mov  [eax+8], ebp
    mov  [eax+12], esp ; save old ESP
    mov  [eax+16], ebx
    mov  [eax+20], edx
    mov  [eax+24], ecx
    mov  [eax+28], eax

    ; Save return address (EIP) to *old_eip
    mov  eax, [esp]
    mov  edx, [esp+12]
    mov  [edx], eax

    ; Load new registers
    mov  eax, [esp+8]
    mov  edi, [eax+0]
    mov  esi, [eax+4]
    mov  ebp, [eax+8]
    mov  esp, [eax+12]
    mov  ebx, [eax+16]
    mov  edx, [eax+20]
    mov  ecx, [eax+24]
    mov  eax, [eax+28]

    ; Jump to new_eip
    mov  edx, [esp+16]
    jmp  edx