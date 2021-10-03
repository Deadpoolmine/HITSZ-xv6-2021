#include "kernel/types.h"
#include "user.h"
/* 

1. //Before you start coding, read Chapter 1 of the xv6 book.
2. //Look at some of the other programs in user/ (e.g., user/echo.c, user/grep.c, and user/rm.c) to see how you can obtain the command-line arguments passed to a program.

3. //!If the user forgets to pass an argument, sleep should print an error message.

4. //!The command-line argument is passed as a string; you can convert it to an integer using atoi (see user/ulib.c).
5. //!Use the system call sleep.

6. //?See kernel/sysproc.c for the xv6 kernel code that implements the sleep system call (look for sys_sleep), user/user.h for the C definition of sleep callable from a user program, and user/usys.S for the assembler code that jumps from user code into the kernel for sleep.

7. //!Make sure main calls exit() in order to exit your program.
8. //!Add your sleep program to UPROGS in Makefile; once you've done that, make qemu will compile your program and you'll be able to run it from the xv6 shell.
9. //Look at Kernighan and Ritchie's book The C programming language (second edition) (K&R) to learn about C. 
*/
int main(int argc,char* argv[]){
    if(argc != 2){            //!If the user forgets to pass an argument, sleep should print an error message.
        printf("Sleep needs one argument!\n"); //检查参数数量是否正确
        exit(-1);
    }
    int ticks = atoi(argv[1]);//!The command-line argument is passed as a string; you can convert it to an integer using atoi
    sleep(ticks);             //!Use the system call sleep
    printf("(nothing happens for a little while)\n");
    exit(0);                  //!Make sure main calls exit() in order to exit your program
}
