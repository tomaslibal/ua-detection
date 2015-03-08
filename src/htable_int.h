#ifndef HTABLE_INT
#define HTABLE_INT

struct htable_int {
    char *name;
    int val;
    struct htable_int *next;
}

struct htable_int *htable_int_create();
void htable_int_set(struct htable_int *table, char *name, int val);
int htable_int_get(struct htable_int *table, char *name);
void htable_int_free(struct htable_int *table);

#endif
