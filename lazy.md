1. You can check whether a fault is a page fault by seeing if r_scause() is 13 or 15 in usertrap().

2. r_stval() returns the RISC-V stval register, which contains the virtual address that caused the page fault.

3. Steal code from uvmalloc() in vm.c, which is what sbrk() calls (via growproc()). You'll need to call kalloc() and mappages().

4. Use PGROUNDDOWN(va) to round the faulting virtual address down to a page boundary.

5. uvmunmap() will panic; modify it to not panic if some pages aren't mapped.

6. If the kernel crashes, look up sepc in kernel/kernel.asm

7. Use your vmprint function from pgtbl lab to print the content of a page table.

8. If you see the error "incomplete type proc", include "spinlock.h" then "proc.h".