# Kompilator C i asembler
CC = /home/mateusz/cross-compiler/bin/x86_64-elf-gcc
AS = nasm
LD = /home/mateusz/cross-compiler/bin/x86_64-elf-ld

# Flagi kompilacji i linkowania
ASFLAGS = -f elf64
CFLAGS = -ffreestanding -mno-red-zone -nostdlib -fPIE -I/usr/include/efi -I/usr/include/efi/x86_64 -DEFI_FUNCTION_WRAPPER
LDFLAGS = -nostdlib -znocombreloc -T linker.ld -L/usr/lib -lefi -lgnuefi

# Struktura katalogów dla ISO
ISO_DIR = iso
EFI_DIR = $(ISO_DIR)/EFI
BOOT_DIR = $(EFI_DIR)/BOOT
BOOTX64 = $(BOOT_DIR)/BOOTX64.EFI
ISO = myos.iso

# Pliki źródłowe i obiektowe
BOOT_SRC = boot_t.asm
KERNEL_SRC = kernel.c
BOOT_OBJ = boot_t.o
KERNEL_OBJ = kernel.o

# Domyślny cel: buduj ISO
all: iso

# Budowanie pliku EFI (bootloader + jądro)
build: $(BOOTX64)

$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(BOOT_DIR)
	$(AS) $(ASFLAGS) $(BOOT_SRC) -o $(BOOT_OBJ)

$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) -c $(KERNEL_SRC) -o $(KERNEL_OBJ)

$(BOOTX64): $(BOOT_OBJ) $(KERNEL_OBJ)
	@mkdir -p $(BOOT_DIR)
	$(LD) $(LDFLAGS) -o $(BOOTX64) $(BOOT_OBJ) $(KERNEL_OBJ)

# Tworzenie bootowalnego obrazu ISO z konfiguracją UEFI
iso: build
	xorriso -as mkisofs \
	    --efi-boot EFI/BOOT/BOOTX64.EFI \
	    --output $(ISO) $(ISO_DIR)

# Uruchomienie w QEMU
run: iso
	qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE_4M.fd -cdrom myos.iso -m 512M -vga std -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_VARS_4M.fd -serial stdio

# Czyszczenie plików tymczasowych
clean:
	rm -f $(BOOT_OBJ) $(KERNEL_OBJ)
	rm -rf $(ISO_DIR) $(ISO)
