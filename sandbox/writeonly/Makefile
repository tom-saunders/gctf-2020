all: shellcode.o chal

chal: LDFLAGS=-static
chal: LDLIBS=-lseccomp
chal: chal.o

sc: shellcode.o
shellcode.o: Makefile shellcode.c
	${CC} -c shellcode.c -o shellcode.o -fPIE -masm=intel -O3


.PHONY: all
