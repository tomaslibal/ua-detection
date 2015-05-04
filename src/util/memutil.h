#ifndef _MEMUTIL_H_
#define _MEMUTIL_H_

/*
 * Checks if *ptr is address 0x0 and if so, exits the program with an error
 * message and prints char *desc as well. 
 */
void chck_malloc(void *ptr, char *desc);

#endif