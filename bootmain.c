#include "types.h"
#include "elf.h"

#define SECTSIZE 512

void readseg(uchar*, uint, uint);

// load kernel elf from disk, elf header first, and then, each elf segments
void bootmain(void) {
    struct elfhdr *elf;
    struct proghdr *ph, *eph;
    void (*entry)(void);
    uchar* pa;

    elf = (struct elfhdr*)0x10000;	// scratch space at pa 0x10000
    readseg((uchar*)elf, 4096, 0);	// read first page off disk

    if(elf->magic != ELF_MAGIC)	return;	// let bootasm.S spin

    ph = (struct proghdr*)((uchar*)elf + elf->phoff);
    eph = ph + elf->phnum;
    for(; ph < eph; ph++) {
	pa = (uchar*)ph->paddr;
	readseg(pa, ph->filesz, ph->off);
	if(ph->memsz > ph->filesz)
	    stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
    }

    // load kernel to physical memory finished, jump to kernel
    entry = (void(*)(void))(elf->entry);
    entry();
}

void waitdisk(void) {
    while((inb(0x1F7) & 0xC0) != 0x40);
}

void readsect(void *dst, uint offset) {
    waitdisk();
    outb(0x1F2, 1);	// count = 1
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);	// cmd 0x20 - read sectors

    // read data
    waitdisk();
    insl(0x1F0, dst, SECTSIZE/4);
}

// read 'count' bytes at 'offset' from kernel into physical address 'pa'
void readseg(uchar* pa, uint count, uint offset) {
    uchar* epa;
    epa = pa + count;
    // round down to sector boundary
    pa -= offset % SECTSIZE;
    // translate form bytes to sectors, kernel starts at sector 1
    offset = (offset / SECTSIZE) + 1;
    for(; pa < epa; pa += SECTSIZE, offset++)
	readsect(pa, offset);
}
