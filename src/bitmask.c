/*
 * bitmask.c
 *
 *  Created on: Mar 21, 2015
 *      Author: tmlbl
 */

#include "bitmask.h"

TYPE mask_is_set(TYPE *mask, CONST_TYPE *flag)
{
    return *mask & *flag;
}

TYPE mask_is_set_bool(TYPE *mask, CONST_TYPE *flag)
{
    TYPE result;

    result = mask_is_set(mask, flag);

    if (result > 0) {
        return 1;
    } else {
        return 0;
    }
}

void mask_set(TYPE *mask, CONST_TYPE *flag)
{
    *mask |= *flag;
}

void mask_unset(TYPE *mask, CONST_TYPE *flag)
{
    *mask &= ~(*flag);
}

