#include <stdio.h>

int X = 2;

void foo_with_param(int Y)
{
    X += Y;
}

void foo_no_param()
{
    X *= 2;
}

int main()
{
    foo_with_param(3);
    foo_no_param();
    printf("%d\n", X);
    return 0;
}