/*
 * bitmask.c
 *
 *  Created on: Mar 21, 2015
 *      Author: tmlbl
 */

#include "bitmask.h"

TYPE mask_is_set(TYPE mask, TYPE flag)
{
    return 0;
}

TYPE mask_is_set_bool(TYPE mask, TYPE flag)
{
    TYPE result;

    result = mask_is_set(mask, flag);

    if (result > 0) {
        return 1;
    } else {
        return 0;
    }
}

void mask_set(TYPE *mask, TYPE *flag)
{
    *mask |= *flag;
}

void mask_unset(TYPE mask, TYPE flag)
{

}

