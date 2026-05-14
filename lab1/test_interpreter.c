#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double interpret(const char *s);

static int total = 0, pass = 0;

void assert_eq(const char *name, const char *expr, double expected)
{
    total++;

    double result = interpret(expr);

    if (fabs(result - expected) < 1e-9)
    {
        printf("PASS  %s\n", name);
        pass++;
    }
    else
    {
        printf("FAIL  %s  expected=%.10g got=%.10g\n",
               name, expected, result);
    }
}

int main()
{
    printf("Arithmetic Interpreter Tests\n\n");

    assert_eq("add", "3 + 5", 8);
    assert_eq("sub", "10 - 3", 7);
    assert_eq("mul", "4 * 3", 12);
    assert_eq("div", "10 / 2", 5);

    assert_eq("precedence1", "2 + 3 * 4", 14);
    assert_eq("precedence2", "10 - 6 / 2", 7);

    assert_eq("paren1", "2 * (4 + 3)", 14);
    assert_eq("paren2", "(1 + 2) * 3", 9);
    assert_eq("paren3", "(2 + 3) * (4 - 1)", 15);

    assert_eq("multi-digit", "100 + 200", 300);
    assert_eq("single", "42", 42);

    printf("\nPassed: %d / %d\n", pass, total);

    return 0;
}