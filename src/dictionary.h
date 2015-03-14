#ifndef DICTIONARY
#define DICTIONARY

struct dict_htable_int {
    char *class_name;
    struct htable_int *root;
    struct dict_htable_int *next;
};

struct dict_htable_int *dict_htable_int_create();

void dict_htable_int_set(
        struct dict_htable_int *node,
        char *class_name,
        struct htable_int *root,
        struct dict_htable_int *next);

struct dict_htable_int *dict_htable_int_find(struct dict_htable_int *node, char *class_name);
struct dict_htable_int *dict_htable_int_find_last(struct dict_htable_int *node);
void dict_htable_int_free(struct dict_htable_int *node);

#endif
