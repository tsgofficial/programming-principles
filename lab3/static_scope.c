#include <stdio.h>

int X = 5; // global

void foo()
{
    int Y = 10;
    printf("%d\n", X + Y);
}

void bar()
{
    int X = 7; // local (shadows global)
    foo();
}

int main()
{
    bar();
    printf("%d\n", X);
    return 0;
}