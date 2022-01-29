// definitions of the x86 memory management unit

// control register flags
#define CR0_PE			0x00000001		// protection enable

// various segment selectors
#define SEG_KCODE 1
#define SEG_KDATA 2
#define SEG_UCODE 3
#define SEG_UDATA 4
#define SEG_TSS   5		// this process's task state
