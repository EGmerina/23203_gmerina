#include <stdio.h>

void hello_from_dynamic_lib()
{
    printf("Hello from dynamic library!\n");
}

//  gcc -fPIC -c dynamic_hello.c
//  gcc -shared -o libdl.so dynamic_hello.o
//  gcc hello.c -L. -ldl -o main


//  echo $LD_LIBRARY_PATH
//  TODO LD_LIBRARY_PATH=./
//  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH