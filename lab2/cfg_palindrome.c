#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RULES 10
#define MAX_BODY 10
#define MAX_STR 256

typedef struct
{
    char head;
    const char *body[MAX_BODY];
    int body_count;
} ProductionRule;

typedef struct
{
    ProductionRule rules[MAX_RULES];
    int rule_count;
    char terminals[10];
    int terminal_count;
    char start_symbol;
} CFG;

int is_terminal(CFG *g, char c)
{
    for (int i = 0; i < g->terminal_count; i++)
        if (g->terminals[i] == c)
            return 1;
    return 0;
}

ProductionRule *find_rule(CFG *g, char symbol)
{
    for (int i = 0; i < g->rule_count; i++)
        if (g->rules[i].head == symbol)
            return &g->rules[i];
    return NULL;
}

void initialize_cfg(CFG *g)
{
    g->rule_count = 1;

    g->rules[0].head = 'S';

    const char *body[] = {
        "aSa", "bSb", "cSc", "dSd",
        "a", "b", "c", "d", ""};

    memcpy(g->rules[0].body, body, sizeof(body));
    g->rules[0].body_count = 9;

    strcpy(g->terminals, "abcd");
    g->terminal_count = 4;
    g->start_symbol = 'S';
}

void generate_palindrome(CFG *g, char symbol, char *out, int depth)
{
    if (depth > 20)
    {
        char c = g->terminals[rand() % g->terminal_count];
        strncat(out, &c, 1);
        return;
    }

    if (is_terminal(g, symbol))
    {
        strncat(out, &symbol, 1);
        return;
    }

    ProductionRule *rule = find_rule(g, symbol);
    if (!rule)
        return;

    const char *prod = rule->body[rand() % rule->body_count];

    for (int i = 0; prod[i]; i++)
        generate_palindrome(g, prod[i], out, depth + 1);
}

int parse_string(CFG *g, const char *s)
{
    int n = strlen(s);

    for (int i = 0; i < n; i++)
        if (!is_terminal(g, s[i]))
            return 0;

    for (int i = 0; i < n / 2; i++)
        if (s[i] != s[n - i - 1])
            return 0;

    return 1;
}

void test_random(CFG *g, int n)
{
    printf("\n=== Random Tests ===\n");

    for (int i = 0; i < n; i++)
    {
        char buf[MAX_STR] = {0};

        generate_palindrome(g, g->start_symbol, buf, 0);

        printf("Test %d: %-15s -> %s\n",
               i + 1,
               buf,
               parse_string(g, buf) ? "VALID" : "INVALID");
    }
}

void test_fixed(CFG *g)
{
    const char *valid[] = {"", "a", "aa", "aba", "abba", "abcdcba"};
    const char *invalid[] = {"ab", "abc", "abca", "xyz"};

    printf("\n=== Valid Cases ===\n");
    for (int i = 0; i < 6; i++)
        printf("%-10s -> %s\n",
               valid[i],
               parse_string(g, valid[i]) ? "VALID" : "INVALID");

    printf("\n=== Invalid Cases ===\n");
    for (int i = 0; i < 4; i++)
        printf("%-10s -> %s\n",
               invalid[i],
               parse_string(g, invalid[i]) ? "INVALID" : "WRONG");
}

int main()
{
    srand(time(NULL));

    CFG g;
    initialize_cfg(&g);

    printf("=== CFG Palindrome Generator & Parser ===\n");

    char buf[MAX_STR] = {0};
    generate_palindrome(&g, g.start_symbol, buf, 0);
    printf("\nGenerated palindrome: %s\n", buf);

    test_random(&g, 5);
    test_fixed(&g);

    char input[MAX_STR];

    while (1)
    {
        printf("\nEnter string (exit to quit): ");
        scanf("%255s", input);

        if (strcmp(input, "exit") == 0)
            break;

        if (parse_string(&g, input))
            printf("VALID\n");
        else
            printf("INVALID\n");
    }

    return 0;
}