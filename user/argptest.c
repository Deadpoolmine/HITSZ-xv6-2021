#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

void argptest(char *s)
{
  int fd;
  fd = open("init", O_RDONLY);
  if (fd < 0) {
    printf("%s: open failed\n", s);
    exit(1);
  }
  void* va = sbrk(0) - 1;
  printf("va: %p", va);
  read(fd, va, -1);
  close(fd);
}

int main(int argc, char const *argv[])
{
    argptest("argptest");
    return 0;
}
