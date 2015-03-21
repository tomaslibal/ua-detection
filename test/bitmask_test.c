/*
 * bitmask_test.c
 *
 *  Created on: Mar 21, 2015
 *      Author: tmlbl
 */

#include "bitmask_test.h"
#include <assert.h>
#include <stdio.h>

#include "test_suite.h"

#include "../src/bitmask.c"

int i_mask = 0;

void test_mask_set()
{
    unsigned short mask = 0;
    const unsigned short FLAG_1 = 1;
    const unsigned short FLAG_2 = 1 << 1;

    mask_set(&mask, &FLAG_1);

    assert(mask != 0);

    mask_set(&mask, &FLAG_2);

    assert(mask == (FLAG_1 + FLAG_2));

    success(i_mask);
}

void test_mask_unset()
{
    unsigned short mask = 0;
    const unsigned short FLAG_1 = 1;
    const unsigned short FLAG_2 = 1 << 1;

    mask_set(&mask, &FLAG_1);
    mask_set(&mask, &FLAG_2);

    mask_unset(&mask, &FLAG_2);

    assert(mask == FLAG_1);

    mask_unset(&mask, &FLAG_1);

    assert(mask == 0);

    success(i_mask);
}

void test_mask_is_set()
{
    unsigned short mask = 0;
    const unsigned short FLAG_1 = 1;
    const unsigned short FLAG_2 = 1 << 1;
    const unsigned short FLAG_3 = 1 << 2;

    mask_set(&mask, &FLAG_1);
    mask_set(&mask, &FLAG_2);

    assert(mask_is_set(&mask, &FLAG_1) > 0);

    assert(mask_is_set(&mask, &FLAG_3) == 0);

    success(i_mask);
}

void test_mask_is_set_bool()
{
    unsigned short mask = 0;
    const unsigned short FLAG_1 = 1;
    const unsigned short FLAG_2 = 1 << 1;
    const unsigned short FLAG_3 = 1 << 2;

    mask_set(&mask, &FLAG_1);
    mask_set(&mask, &FLAG_2);

    assert(mask_is_set_bool(&mask, &FLAG_1) == 1);

    assert(mask_is_set_bool(&mask, &FLAG_3) == 0);

    success(i_mask);
}

int run_test_bitmask()
{
    test_mask_set();
    test_mask_unset();
    test_mask_is_set();
    test_mask_is_set_bool();

    printf("\n");
    return i_mask;
}
