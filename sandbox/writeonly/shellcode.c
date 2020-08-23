#include <unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/stat.h>
void sc() {
    uint32_t zero = 0;
    uint32_t four = 4;
    uint32_t sixteen = 16;

    uint32_t ret = 0;
    uint32_t fd_1 = 1;
    uint32_t pid_2 = 2;
    uint32_t err = 0;

    uint32_t nocare = 0;

    struct stat stat_out;

    asm volatile (
      "movabs %%rax, 0x0067616c662f7265\n\t"
      "push %%rax\n\t"
      "movabs %%rax, 0x73752f656d6f682f\n\t"
      "push %%rax"
      :
      :
      : "rax", "memory");
    asm volatile (
      "mov %%esi, %%esp\n\t"
      "syscall"
      : "=a"(ret), "=d"(err)
      : "a"(__NR_write), "D"(fd_1), "d"(sixteen)
      : "esi", "memory");
    asm volatile (
      "mov %%edi, %%esp\n\t"
      "syscall"
      : "=a"(ret), "=d"(err)
      : "a"(__NR_open), "S"(zero), "d"(zero)
      : "edi", "memory");
    asm volatile (
      "syscall"
      : "=a"(nocare)
      : "a"(__NR_write), "D"(fd_1), "S"(&ret), "d"(four)
      : "memory");
    asm volatile (
      "syscall"
      : "=a"(nocare)
      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(four)
      : "memory");
    asm volatile (
      "syscall"
      : "=a"(ret), "=d"(err)
      : "a"(__NR_fstat), "D"(ret), "S"(&stat_out)
      : "memory");
//    asm volatile (
//      "syscall"
//      : "=a"(r)
//      : "a"(__NR_clone), "D"(0), "S"(0)
//      : "memory");
//    asm volatile (
//      "syscall"
//      :
//      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(4)
//      : "memory");
    asm volatile (
      "syscall"
      : "=a"(nocare)
      : "a"(__NR_write), "D"(fd_1), "S"(&ret), "d"(4)
      : "memory");
    asm volatile (
      "syscall"
      : "=a"(nocare)
      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(4)
      : "memory");
    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(fd_1), "S"(&(stat_out.st_size)), "d"(sizeof(off_t))
      : "memory");
}

