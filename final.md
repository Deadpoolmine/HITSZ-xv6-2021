-[x] Handle negative sbrk() arguments.

-[x] Kill a process if it page-faults on a virtual memory address higher than any allocated with sbrk().

-[x] Handle the parent-to-child memory copy in fork() correctly.

-[ ] Handle the case in which a process passes a valid address from sbrk() to a system call such as read or write, but the memory for that address has not yet been allocated.

-[x] Handle out-of-memory correctly: if kalloc() fails in the page fault handler, kill the current process.

-[x] Handle faults on the invalid page below the user stack.