ASM = nasm
CC = gcc
LD = ld

all: os-image.bin

boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

kernel.o: kernel.c
	$(CC) -ffreestanding -m32 -fno-pic -c kernel.c -o kernel.o

kernel_entry.o: kernel.asm
	$(ASM) -f elf32 kernel.asm -o kernel_entry.o

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -Ttext 0x1000 -o kernel.elf kernel_entry.o kernel.o
	objcopy -O binary kernel.elf kernel.bin

os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin

clean:
	rm -f *.bin *.o *.elf