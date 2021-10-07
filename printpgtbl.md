1. You can put vmprint() in kernel/vm.c.
2. Use the macros at the end of the file kernel/riscv.h.
3. The function freewalk may be inspirational.
4. Define the prototype for vmprint in kernel/defs.h so that you can call it from exec.c.
5. Use %p in your printf calls to print out full 64-bit hex PTEs and addresses as shown in the example.