#define EXTMEM	0x100000		// start of extended memory
#define PHYSTOP	0xE000000		// top physical memory
#define DEVSPACE 0xFE000000		// other devices are at high addresses

#define KERNBASE 0x80000000		// first kernel vitrual address
#define KERNLINK (KERNBASE+EXTMEM)	// address where kernel is linked


