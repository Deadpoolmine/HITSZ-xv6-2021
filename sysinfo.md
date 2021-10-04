1. Add $U/_sysinfotest to UPROGS in Makefile

2. Run make qemu; user/sysinfotest.c will fail to compile. Add the system call sysinfo, following the same steps as in the previous assignment. To declare the prototype for sysinfo() in user/user.h you need predeclare the existence of struct sysinfo:

    struct sysinfo;
    int sysinfo(struct sysinfo *);
  
3. Once you fix the compilation issues, run sysinfotest; it will fail because you haven't implemented the system call in the kernel yet.

4. sysinfo needs to copy a struct sysinfo back to user space; see sys_fstat() (kernel/sysfile.c) and filestat() (kernel/file.c) for examples of how to do that using copyout().

5. To collect the amount of free memory, add a function to kernel/kalloc.c

6. To collect the number of processes, add a function to kernel/proc.c