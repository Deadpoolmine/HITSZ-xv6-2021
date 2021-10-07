1. You can perform the mapping in proc_pagetable() in kernel/proc.c.

2. Choose permission bits that allow userspace to only read the page.

3. You may find that mappages() is a useful utility.

4. Don't forget to allocate and initialize the page in allocproc().

5. Make sure to free the page in freeproc().