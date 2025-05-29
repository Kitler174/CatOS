.set MB2_MAGIC,     0xE85250D6
.set MB2_ARCH,      0                  # i386
.set MB2_HEADERLEN, header_end - mb2_header
.set MB2_CHECKSUM,  -(MB2_MAGIC + MB2_ARCH + MB2_HEADERLEN)

.section .multiboot
.align 8
mb2_header:
    .long MB2_MAGIC
    .long MB2_ARCH
    .long MB2_HEADERLEN
    .long MB2_CHECKSUM

    # --- Framebuffer tag (type=5) ---
    .short 5         # type
    .short 0         # flags
    .long 24         # size (24 bytes including header and padding)
    .long 1024       # width
    .long 768        # height
    .long 32         # depth (bpp)
    .long 0          # PADDING (4 bajty paddingu, żeby mieć 24 bajty)

    # --- End tag (type=0) ---
    .short 0         # type
    .short 0         # reserved
    .long 8          # size

header_end:

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
    lea kernel_stack_end, %esp
    call callConstructors
    push %ebx          # pointer to multiboot2 info struct
    push %eax          # magic
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop

.section .bss
    .space 131072
kernel_stack_end: