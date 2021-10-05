1. You can use the constant NCPU from kernel/param.h
2. Let freerange give all free memory to the CPU running freerange.
3. The function cpuid returns the current core number, but it's only safe to call it and use its result when interrupts are turned off. You should use push_off() and pop_off() to turn interrupts off and on.
4. Have a look at the snprintf function in kernel/sprintf.c for string formatting ideas. It is OK to just name all locks "kmem" though.