CFLAGS = -fno-pic -no-pie -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -fno-omit-frame-pointer
CFLAGS += $(shell gcc -fno-stack-protector -E -x c /dev/null > /dev/null 2>&1 && echo -fno-stack-protector)
ASFLAGS = -m32 -gdwarf-2 -Wa,-divide
LDFLAGS = -m elf_i386

bootblock: bootasm.S bootmain.c
	gcc $(CFLAGS) -fno-pic -O -nostdinc -I. -c bootmain.c
	gcc $(CFLAGS) -fno-pic -nostdinc -I. -c bootasm.S
	ld $(LDFLAGS) -N -e start -Ttext 0x7000 -o bootblock.o bootasm.o bootmain.o
	objcopy -S -O binary -j .text bootblock.o bootblock
