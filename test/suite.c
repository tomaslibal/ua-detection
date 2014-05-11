#include "suite.h"

int done(int n, int pass)
{
    printf("\n\nTests: passing %d, failing %d, total %d\n", pass, n-pass, n);
    if (pass == n)
        printf("ALL PASSED!\n");
    return n-pass;
}
