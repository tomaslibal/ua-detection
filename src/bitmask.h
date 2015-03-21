#ifndef BITMASK_HEADER
#define BITMASK_HEADER

#define TYPE unsigned short
#define CONST_TYPE const TYPE

TYPE mask_is_set(TYPE *mask, CONST_TYPE *flag);
TYPE mask_is_set_bool(TYPE *mask, CONST_TYPE *flag);
void mask_set(TYPE *mask, CONST_TYPE *flag);
void mask_unset(TYPE *mask, CONST_TYPE *flag);

#endif
