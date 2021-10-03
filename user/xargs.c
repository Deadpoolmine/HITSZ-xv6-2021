#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXARG_LEN 32
#define MAX_CONTENT 128
/* 
1. //! Use fork and exec to invoke the command on each line of input. Use wait in the parent to wait for the child to complete the command.
2. //! To read individual lines of input, read a character at a time until a newline ('\n') appears.
3. kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.
4. Add the program to UPROGS in Makefile.
5. Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
 */
int main(int argc, char const *argv[])
{
    char *args[MAXARG + 2];
    char buf[MAX_CONTENT] = { 0 };
    int i = 0, index = 0;
    int pid;
    
    for (i = 0; i < MAXARG + 2; i++)
    {
        args[i] = (char *)malloc(MAXARG_LEN);
        memset(args[i], 0, MAXARG_LEN);
    }
    
    for (i = 1; i < argc; i++)
    {
        memcpy(args[index++], argv[i], strlen(argv[i]));
    }

    while (read(0, buf, MAX_CONTENT) != 0) {
        char *p = buf;
        char *pe = p;

        while (*pe != '\0')
        {
            if (*pe == '\n') { 
                memset(args[index], 0, MAXARG_LEN);
                memcpy(args[index++], p, pe - p);
                p = pe + 1;
            }
            pe++;
        }

        if ((pid = fork()) == 0 ){
            args[index] = 0; 
            exec(args[0], args);
        }
        else {
            index = argc - 1;
            memset(buf, 0, MAX_CONTENT);
            wait(&pid);
        }
    }
    exit(0);
}
