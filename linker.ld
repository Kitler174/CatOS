ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 1M;

    .text : {
        *(.multiboot2)     /* Multiboot2 header (zalecane, choć .multiboot też działa) */
        *(.multiboot)      /* Dla kompatybilności, jeśli masz też nagłówek M1 */
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        start_ctors = .;
        KEEP(*(.init_array*))
        end_ctors = .;
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ : {
        *(.fini_array*)
        *(.comment)
    }
}