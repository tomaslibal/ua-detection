#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int num_tests = 0;
int p = 0;
#include "suite.h"

#include "../src/uas.h"

/* clang ../src/utils.c suite.c test_utils.c */

int main()
{
    printf("Executing test case 'test_uas'\n");

    /* tokenization tests */
    char **k = NULL;
    int cnt = tokenize(&k, "Mozilla/5.0");
    ASSERT("Expect cnt = 0", cnt==0);

    ASSERT("Expect k[0] = 'Mozilla/5.0'", 0==strcmp(k[0],"Mozilla/5.0"));

    return done(num_tests, p);
}
