// To see the expanded macros: 
//   $ gcc -E sizeof.c
// To compile the program:
//   $ gcc -Wall sizeof.c
#include <stdio.h>

// 1. #x can concat strings
// 2. %lu for unsigned long int (aka. size_t)
// 3. \ to join the lines
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

// Count the number of elements in an array x.
#define countof(x)  (sizeof(x)/sizeof(x[0]))

// Not work as expected.
void bar(void) {
    const char *types[] = {
       "char",
       "short",
       "int",
       "long",
       "float",
       "double"
    };

    // 1. sizeof(types) is the bytes of the array types[].
    // 2. sizeof(types[0]) is the bytes of an element of array: types[0]
    for (size_t i = 0; i < countof(types); i++) {
        SZ(types[i]);
    }

    // Not worked either.
    for (size_t i = 0; i < countof(types); i++) {
        printf("sizeof(%s) = %lu\n", types[i], sizeof(types[i]));
    }
}

int main(void) {
    foo();
    bar();
    return 0;
}

