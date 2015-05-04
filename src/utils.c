#include "util/logging.h"
#include "util/memutil.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const char logm_separator = '\n';

void logmsg(char *msg, char *src)
{
 
}

/*
 * malloc can return a NULL pointer if the allocation fails
 *
 * If *ptr is a pointer that has just been malloc'd and passed to this function
 * it will print an error and exit if *ptr is a NULL pointer.
 */
void chck_malloc(void *ptr, char *desc)
{
    if (ptr == NULL) {
        printf("Error malloc'ing for %s\n", desc);
        exit(1);
    }
}