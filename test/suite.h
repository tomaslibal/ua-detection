#ifndef SUITE
#define SUITE

#include <assert.h>
#include <stdio.h>

extern int num_tests;
extern int p;
extern int f;

#define ASSERT(msg, ...) \
    num_tests++; \
    if(__VA_ARGS__) { fprintf(stdout, "."); p++; }else{ fprintf(stdout, "\nF: "); fprintf(stdout, "(%s:%d) %s\n", __FILE__, __LINE__, msg); }

/* call this when all test done,
    returns number of failing tests
 */
int done(int n, int p);

#endif
