#include <string.h>
#include <stdio.h>

int strncmp(char const *s1 , char const *s2, size_t n) {
    char b1[128];
    char b2[128];
    int ls1 = strnlen(s1, n);
    int ls2 = strnlen(s2, n);
    strncpy(b1, s1, ls1 <= 127 ? ls1 : 127);
    strncpy(b2, s2, ls2 <= 127 ? ls2 : 127);
    int r = strcmp(b1, b2);
    printf("\nstrncmp\n[%lu] [%d] [%d]\n[%s]\n[%s]\n[%d]\n", n, ls1, ls2, b1, b2, r);
    return r;
}
