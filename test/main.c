#include <assert.h>
#include <stdio.h>

#include "htable_int_test.h"
#include "tokenizer_test.h"
#include "reader_test.h"
#include "probab_test.h"
#include "dictionary_test.h"
#include "bitmask_test.h"

int main()
{
    printf("Running the test suites\n");

    run_test_htable_int();
    run_test_tokenizer();
    run_test_reader();
    run_test_probab();
    run_test_dictionary();
    run_test_bitmask();
    return 0;
}
