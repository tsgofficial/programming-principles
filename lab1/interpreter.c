#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512

char **tokenize(const char *s, int *out_n)
{
    char **tok = malloc(sizeof(char *) * MAX);
    int n = 0;

    *out_n = 0;

    char prev = ' ';

    while (*s)
    {
        if (isspace(*s))
        {
            prev = ' ';
        }
        else if (strchr("0123456789", *s) && strchr("0123456789", prev))
        {
            int len = strlen(tok[n - 1]);
            tok[n - 1] = realloc(tok[n - 1], len + 2);
            tok[n - 1][len] = *s;
            tok[n - 1][len + 1] = '\0';
        }
        else if (strchr("0123456789", *s) || strchr("+-*/()", *s))
        {
            char *c = malloc(2);
            c[0] = *s;
            c[1] = '\0';
            tok[n++] = c;
        }

        prev = *s;
        s++;
    }

    *out_n = n;
    return tok;
}

typedef struct
{
    char **t;
    int n;
    int i;
} P;

char *peek(P *p)
{
    return p->i < p->n ? p->t[p->i] : NULL;
}

char *next(P *p)
{
    return p->t[p->i++];
}

double expr(P *p);

double factor(P *p)
{
    char *x = peek(p);

    if (x[0] == '(')
    {
        next(p);
        double v = expr(p);
        if (peek(p))
            next(p);
        return v;
    }

    return atof(next(p));
}

double term(P *p)
{
    double v = factor(p);

    while (peek(p) && (peek(p)[0] == '*' || peek(p)[0] == '/'))
    {
        char op = next(p)[0];
        double r = factor(p);

        v = (op == '*') ? v * r : v / r;
    }

    return v;
}

double expr(P *p)
{
    double v = term(p);

    while (peek(p) && (peek(p)[0] == '+' || peek(p)[0] == '-'))
    {
        char op = next(p)[0];
        double r = term(p);

        v = (op == '+') ? v + r : v - r;
    }

    return v;
}

double interpret(const char *s)
{
    int n = 0;
    char **tok = tokenize(s, &n);

    if (!tok || n == 0)
        return 0;

    P p = {tok, n, 0};
    double result = expr(&p);

    for (int i = 0; i < n; i++)
        free(tok[i]);
    free(tok);

    return result;
}

#ifndef TESTING
int main()
{
    char b[256];

    printf("Interpreter (exit)\n");

    while (1)
    {
        printf("> ");
        fgets(b, sizeof(b), stdin);

        if (!strncmp(b, "exit", 4))
            break;

        printf("%g\n", interpret(b));
    }
}
#endif