all: shellcode.o chal payload.out

chal: LDFLAGS=-static
chal: LDLIBS=-lseccomp
chal: chal.o

payload.out: shellcode.o
	objcopy -O binary --only-section .text shellcode.o payload.tmp
	du -bs payload.tmp | cut -f 1 > payload.len
	cat payload.len payload.tmp > payload.out
	rm payload.len payload.tmp


payload.odump: shellcode.o
	objdump -Mintel -d shellcode.o > payload.odump


shellcode.o: Makefile shellcode.c
	${CC} -c shellcode.c -o shellcode.o -fPIE -masm=intel -O3

local: payload.out
	cat payload.out | ./chal > system.response || true
	xxd system.response
doit: payload.out
	cat payload.out | ncat writeonly.2020.ctfcompetition.com 1337 > system.response
	xxd system.response

.PHONY: all doit local
