#include <assert.h>
#include <stdio.h>

#include "htable_int_test.h"
#include "tokenizer_test.h"
#include "reader_test.h"

int main()
{
    run_test_htable_int();
    run_test_tokenizer();
    run_test_reader();
    return 0;
}
