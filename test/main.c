#include <assert.h>
#include <stdio.h>

#include "htable_int_test.h"
#include "tokenizer_test.h"
#include "reader_test.h"
#include "probab_test.h"
#include "dictionary_test.h"
#include "bitmask_test.h"

/*
 * Total Successful # of tests
 */
int ts = 0;

int main()
{
    printf("Running the test suites\n");

    ts += run_test_htable_int();
    ts += run_test_tokenizer();
    run_test_reader();
    run_test_probab();
    run_test_dictionary();
    run_test_bitmask();

    printf("\n%d tests successfully passed\n", ts);
    return 0;
}
