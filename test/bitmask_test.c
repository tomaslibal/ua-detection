/*
 * bitmask_test.c
 *
 *  Created on: Mar 21, 2015
 *      Author: tmlbl
 */

#include "bitmask_test.h"
#include <assert.h>

#include "../src/bitmask.c"

void test_mask_set()
{
    unsigned short mask = 0;
    const unsigned short FLAG_1 = 1;
    const unsigned short FLAG_2 = 1 << 1;

    mask_set(&mask, &FLAG_1);

    assert(mask != 0);

    mask_set(&mask, &FLAG_2);

    assert(mask == (FLAG_1 + FLAG_2));
}

void run_test_bitmask()
{
    test_mask_set();
}
