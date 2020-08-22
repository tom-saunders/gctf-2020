#include <string.h>
#include <stdio.h>

int strncmp(char const *s1 , char const *s2, size_t n) {
    unsigned char b1[17];
    unsigned char b2[17];
    unsigned int ls1 = strnlen(s1, n);
    unsigned int ls2 = strnlen(s2, n);
    strncpy(b1, s1, 16);
    b1[16] = '\0';
    strncpy(b2, s2, 16);
    b2[16] = '\0';
    int r = strcmp(b1, b2);
    printf("\n[0x");
    fflush(0);
    for (int i = 0; i < 16; ++i) {
        printf("%02x", (unsigned int) b1[i]);
        if ((i % 4) == 3) {
            printf(" ");
        }
        fflush(0);
    }
    printf("]\n");
    printf("[0x");
    fflush(0);
    for (int i = 0; i < 16; ++i) {
        printf("%02x", (unsigned int) b2[i]);
        if ((i % 4) == 3) {
            printf(" ");
        }
        fflush(0);
    }
    printf("] ");
    printf("[%d]\n", r);
    return r;
}
