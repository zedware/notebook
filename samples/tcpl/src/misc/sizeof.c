// To see the expanded macros: 
//   $ gcc -E sizeof.c
// To compile the program:
//   $ gcc -Wall sizeof.c
#include <stdio.h>

#define SZ(x)  { \
    printf("sizeof(" #x ") = %lu\n", sizeof(x)); \
  }

void foo(void) {
    SZ(char);
    SZ(short);
    SZ(int);
    SZ(long);
    SZ(float);
    SZ(double);
}

int main(void) {
    foo();
    return 0;
}
