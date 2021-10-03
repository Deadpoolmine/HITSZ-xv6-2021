#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
/* Tips: 
1. //!Use pipe to create a pipe.
2. //!Use fork to create a child.
3. Use read to read from the pipe, and write to write to the pipe.
4. Use getpid to find the process ID of the calling process.
5. Add the program to UPROGS in Makefile.
6. User programs on xv6 have a limited set of library functions available to them. You can see the list in user/user.h; the source (other than for system calls) is in user/ulib.c, user/printf.c, and user/umalloc.c. 
*/
/* output
    4: received ping            说明子进程先读
    3: received pong
 */
int main(int argc, char const *argv[])
{
    int pip_ping[2];
    int pip_pong[2];
    
    int pip[2];

    int pid;
    /* TODO：能否用一个管道实现？不行  */
    /* 用两个管道实现 */
    pipe(pip_ping);                          /* 为我们创建了一个管道 */   //!Use pipe to create a pipe.
    pipe(pip_pong);
    pipe(pip);

    char buf[10];
    memset(buf, 0, 10);

    if ((pid = fork()) == 0) {          /* 子进程返回0 */           //!Use fork to create a child
        close(pip_ping[1]);
        read(pip_ping[0], buf, 10);
        printf("%d: received %s\n", getpid(), buf);
        
        close(pip_pong[0]);
        write(pip_pong[1], "pong", strlen("pong"));

        close(pip_ping[0]);
        close(pip_pong[1]);
    }
    else {                              /* 父进程返回非0 */
        close(pip_ping[0]);
        write(pip_ping[1], "ping", strlen("ping"));
        
        close(pip_pong[1]);
        read(pip_pong[0], buf, 10);
        printf("%d: received %s\n", getpid(), buf);
        
        close(pip_ping[1]);
        close(pip_pong[0]);
    }
    
    /* 一个管道只能实现单方面传输 */
    // if ((pid = fork()) == 0) {              /* 子进程返回0 */           //!Use fork to create a child
    //     memset(buf, 0, 10);

    //     close(pip[1]);                      /* 重新打开如何？ */
    //     read(pip[0], buf, 10);
        
    //     close(pip[0]);
    //     write(pip[1], "pong", strlen("pong"));
    //     printf("%d: received %s\n", getpid(), buf);
    // }   
    // else {
    //     memset(buf, 0, 10);
        
    //     close(pip[0]);
    //     write(pip[1], "ping", strlen("ping"));
        
    //     close(pip[1]);
    //     read(pip[0], buf, 10);
    //     printf("%d: received %s\n", getpid(), buf);
    // }
    exit(0);                                /* 一定要调用exit */
}
