#include <stdio.h>

int X = 5;

void foo()
{
    printf("%d\n", X);
}

void bar()
{
    int X = 10;
    foo();
}

int main()
{
    bar();
    return 0;
}