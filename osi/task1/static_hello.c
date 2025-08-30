#include <stdio.h>

void hello_from_static_lib()
{
    printf("Hello from static library!\n");
}

// gcc -c static_hello.c
// ar rc libsl.a static_hello.o
// ranlib libsl.a
// gcc hello.c -L. -lsl -o main
