#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/* 
1. //!Look at user/find.c to see how to read directories.
2. Use recursion to allow find to descend into sub-directories.
3. //!Don't recurse into "." and "..".
4. Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
5. You'll need to use C strings. Have a look at K&R (the C book), for example Section 5.5.
6. Note that == does not compare strings like in Python. Use strcmp() instead.
7. Add the program to UPROGS in Makefile.

 */

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);

    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(char* path, char* target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        if (memcmp(fmtname(path), target, strlen(target)) == 0) { 
            // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            if (memcmp(p, ".", strlen(".")) == 0 || 
                memcmp(p, "..", strlen("..")) == 0) {
                continue;
            }
            
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            find(buf, target);
            // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        }
        break;
    }
    close(fd);
}

int main(int argc, char const *argv[])
{
    char* path = (char *)argv[1];
    char* target = (char *)argv[2];
    find(path, target);
    exit(0);
}
