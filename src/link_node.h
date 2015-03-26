#ifndef LINK_NODE_H
#define LINK_NODE_H

#ifndef NODE_TYPE
#define NODE_TYPE int
#endif

#define CONCAT(x, y) x ## y
#define AUX(A, B) CONCAT(A, B)
#define LINK_NODE_NAME AUX(link_node_, NODE_TYPE)

struct LINK_NODE_NAME {
    char *name;
    NODE_TYPE val;
    struct LINK_NODE_NAME *next;
};

struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _create) ();
void AUX(LINK_NODE_NAME, _set)(struct LINK_NODE_NAME *table, char *name, NODE_TYPE val);
NODE_TYPE AUX(LINK_NODE_NAME, _get_val)(struct LINK_NODE_NAME *table, char *name);
struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _get)(struct LINK_NODE_NAME *root, char *name);
struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _get_last)(struct LINK_NODE_NAME *root);
void AUX(LINK_NODE_NAME, _free)(struct LINK_NODE_NAME *table);
NODE_TYPE AUX(LINK_NODE_NAME, _sum_val_rec)(struct LINK_NODE_NAME *node);

#endif
