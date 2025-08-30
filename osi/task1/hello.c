#include <stdio.h>

int main(void)
{
  //  printf("Hello, World!");
  //  hello_from_static_lib();
    hello_from_dynamic_lib();
}

// gcc hello.c -L. -ldl -lsl -o main