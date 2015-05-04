#ifndef NODE_TYPE
#    define NODE_TYPE int
#endif
#ifndef CONCAT
#    define CONCAT(x, y) x ## y
#endif
#ifndef FUNC
#    define FUNC(A, B) CONCAT(A, B)
#endif
#ifndef LINK_STRUCT_NAME
#    define LINK_STRUCT_NAME FUNC(link_node_, NODE_TYPE)
#endif

#ifndef LINK_NODE_INT
#define LINK_NODE_INT
struct link_node_int {
    char *name;
    int val;
    struct link_node_int *next;
};
#endif

#ifndef LINK_NODE_FLOAT
#define LINK_NODE_FLOAT
struct link_node_float {
    char *name;
    float val;
    struct link_node_float *next;
};
#endif

struct LINK_STRUCT_NAME *FUNC(LINK_STRUCT_NAME, _create)      ();
void                     FUNC(LINK_STRUCT_NAME, _set)         (struct LINK_STRUCT_NAME *node, char *name, NODE_TYPE val);
NODE_TYPE                FUNC(LINK_STRUCT_NAME, _get_val)     (struct LINK_STRUCT_NAME *node, char *name);
struct LINK_STRUCT_NAME *FUNC(LINK_STRUCT_NAME, _get)         (struct LINK_STRUCT_NAME *head, char *name);
struct LINK_STRUCT_NAME *FUNC(LINK_STRUCT_NAME, _get_last)    (struct LINK_STRUCT_NAME *head);
void                     FUNC(LINK_STRUCT_NAME, _free)        (struct LINK_STRUCT_NAME *node);
NODE_TYPE                FUNC(LINK_STRUCT_NAME, _sum_val_rec) (struct LINK_STRUCT_NAME *node);

#undef NODE_TYPE
#undef LINK_STRUCT_NAME