#include <stdlib.h>
#include <stdio.h>

int num_tests = 0;
int p = 0;
#include "suite.h"

#include "../src/utils.h"

/* clang ../src/utils.c suite.c test_ann.c */

int main()
{
    printf("Executing test case 'test_ann'\n");

    /* sample_mean test */
    double *w = calloc(3, sizeof(double));
    w[0] = 1.0;
    w[1] = 2.0;
    w[2] = 3.0;
    double r = sample_mean(w, 3);
    ASSERT("Expect the sample_mean = 2", r==2.0);

    done(num_tests, p);
    return 0;
}
