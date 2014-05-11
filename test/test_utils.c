#include <stdlib.h>
#include <stdio.h>

int num_tests = 0;
int p = 0;
#include "suite.h"

#include "../src/utils.h"

/* clang ../src/utils.c suite.c test_utils.c */

int main()
{
    printf("Executing test case 'test_utils'\n");

    /* sample_mean test */
    double *w = calloc(3, sizeof(double));
    w[0] = 1.0;
    w[1] = 2.0;
    w[2] = 3.0;
    double r = sample_mean(w, 1);
    ASSERT("Expect sample_mean = 1", r==1.0);

    r = sample_mean(w, 3);
    ASSERT("Expect sample_mean = 2", r==2.0);

    /* sample_std_test */

    double o = sample_std_dev(w, 3, r);
    ASSERT("Expect sample_std_dev = 1", o==1.0);

    return done(num_tests, p);
}
