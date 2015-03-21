#ifndef BITMASK_HEADER
#define BITMASK_HEADER

#define TYPE unsigned short

TYPE mask_is_set(TYPE mask, TYPE flag);
TYPE mask_is_set_bool(TYPE mask, TYPE flag);
void mask_set(TYPE mask, TYPE flag);
void mask_unset(TYPE mask, TYPE flag);

#endif
