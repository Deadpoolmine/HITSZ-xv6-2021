#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
/* 
1. Be careful to close file descriptors that a process doesn't need, because otherwise your program will run xv6 out of resources before the first process reaches 35.
2. Once the first process reaches 35, it should wait until the entire pipeline terminates, including all children, grandchildren, &c. Thus the main primes process should only exit after all the output has been printed, and after all the other primes processes have exited.
3. //!Hint: read returns zero when the write-side of a pipe is closed. -1
4. It's simplest to directly write 32-bit (4-byte) ints to the pipes, rather than using formatted ASCII I/O.
5. You should create the processes in the pipeline only as they are needed.
6. Add the program to UPROGS in Makefile.
*/

/* 实现原理：
p = get a number from left neighbor
print p
loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
 */

void prime_selector(int n, int* pipeline, int size) {
    int pid;
    int pip[2];
    int p;
    int next_pipeline[34], next_size = 0; 
    int ret;
    pipe(pip);

    if ((pid = fork()) == 0) {                      /* Child , receiver */
        close(pip[1]);                              /* 关闭写端 */
        ret = read(pip[0], &p, sizeof(int));

        if (ret > 0) {
            printf("prime %d \n", p);
        }

        while(ret > 0) {
            ret = read(pip[0], &p, sizeof(int));    /* 读 */
            if (ret > 0) {
                if (p % n != 0) {
                    next_pipeline[next_size++] = p;
                }
            }
        }

        if (next_size != 0) {
            prime_selector(++n, next_pipeline, next_size);
        }

        close(pip[0]);
        exit(0);
    }
    else {                                /* Parent , sender */
        close(pip[0]);

        write(pip[1], pipeline, sizeof(int) * size);
        
        close(pip[1]);
        wait(&pid);
        exit(0);
    }
} 

int main(int argc, char const *argv[])
{
    int pipeline[34], i;
    for (i = 0; i < 34; i++)
    {
        pipeline[i] = i + 2;
    }
    prime_selector(2, pipeline, 34);
    exit(0);
}
