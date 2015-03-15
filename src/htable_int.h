#ifndef HTABLE_MODULE
#define HTABLE_MODULE

#ifndef HTABLE_TYPE
#define HTABLE_TYPE int
#endif

#define CONCAT(x, y) x ## y
#define AUX(A, B) CONCAT(A, B)
#define HTABLE_NAME AUX(htable_, HTABLE_TYPE)

struct HTABLE_NAME {
    char *name;
    HTABLE_TYPE val;
    struct HTABLE_NAME *next;
};

struct HTABLE_NAME *AUX(HTABLE_NAME, _create) ();
void AUX(HTABLE_NAME, _set)(struct HTABLE_NAME *table, char *name, HTABLE_TYPE val);
HTABLE_TYPE AUX(HTABLE_NAME, _get_val)(struct HTABLE_NAME *table, char *name);
struct HTABLE_NAME *AUX(HTABLE_NAME, _get)(struct HTABLE_NAME *root, char *name);
struct HTABLE_NAME *AUX(HTABLE_NAME, _get_last)(struct HTABLE_NAME *root);
void AUX(HTABLE_NAME, _free)(struct HTABLE_NAME *table);
HTABLE_TYPE AUX(HTABLE_NAME, _sum_val_rec)(struct HTABLE_NAME *node);

#endif
