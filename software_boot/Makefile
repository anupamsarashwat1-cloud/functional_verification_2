CC=riscv64-unknown-elf-gcc
OBJCOPY=riscv64-unknown-elf-objcopy
CFLAGS=-march=rv64gc -mabi=lp64d -mcmodel=medany -ffreestanding -O0 -nostdlib -nostartfiles -T linker.ld

all: boot.mem

boot.elf: start.S main.c
	$(CC) $(CFLAGS) -o boot.elf start.S main.c

boot.bin: boot.elf
	$(OBJCOPY) -O binary boot.elf boot.bin

boot.mem: boot.bin
	hexdump -v -e '1/4 "%08x" "\n"' boot.bin > boot.mem

clean:
	rm -f *.elf *.bin *.mem
