/*
 * ua_db.c
 *
 * ua_db is a program that stores, labels and retrieves user-agent string data.
 *
 *  Created on: Mar 26, 2015
 *      Author: tmlbl
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "btree.c"
#include "link_node.c"

void read_cli_arguments(int argc, char **argv);
void print_usage();
char *serialize_btree(struct bNode *root, char *out, int *len, int *num);
char *serialize_bnode(struct bNode *node, char *out, int *len, int *num);
void save_db(char *dbf, struct bNode *root);
void load_db(char *dbf, struct bNode *root);
void unserialize();
void add_uas(struct bNode *root, char *uas);
void find_uas(struct bNode *root, char *uas);
void add_class(struct bNode *root, char *uas, char *class);
void print_btree(struct bNode *root);
void print_link_node_int(struct link_node_int *node);

/*
 * The serialized binary tree strings (User-agent string and the classes)
 * will be separated by this char
 */
const char STR_SEPARATOR = 10;
/*
 * The serialized binary tree nodes will be separated by this char
 */
const char NODE_SEPARATOR = 0;

/*
 * Database file for the user agent strings
 */
char *dbf = "data/ua.db.new";

/*
 * Data are stored in memory in a binary tree and this is the root node
 * of that data structure.
 */
struct bNode *root = NULL;

int main(int argc, char **argv)
{

    /*
     * Usage
     *
     *     ua_db add "Mozilla/5.0 (Linux; Android) Firefox/35.0"
     *     ua_db get "Mozilla/5.0 ..."
     *     ua_db add-label <uas-id> label1[, label2, ...]
     *     ua_db remove-label <uas-id> label1[, label2, ...]
     */

    root = bNode_create();

    load_db(dbf, root);

    read_cli_arguments(argc, argv);

    //print_btree(root);

    save_db(dbf, root);

    bNode_free(root);

    return 0;
}

/*
 * Prints the strings of each node in the binary tree whose root node
 * is passed in
 */
void print_btree(struct bNode *root)
{
    if (root == NULL) {
        return;
    }

    if (root->uas != NULL)
        printf("%s:", root->uas);

    print_link_node_int(root->classes);

    printf("\n");

    print_btree(root->left);
    print_btree(root->right);
}

void print_link_node_int(struct link_node_int *node)
{
    if (node == NULL) {
        return;
    }

    if (node->name != NULL)
        printf("%s", node->name);

    print_link_node_int(node->next);
}

void read_cli_arguments(int argc, char **argv)
{
    int c;
    char *tmp = NULL;
    char *uas = NULL;

    static struct option long_options[] = {
            { "add", required_argument, 0, 'a' },
            { "get", required_argument, 0, 'g' },
            { "update", required_argument, 0, 'u' },
            { "add-class", required_argument, 0, 'l' },
            { "remove-class", required_argument, 0, 'r' },
            { "help", no_argument, 0, 'h' }
     };

     while (1) {
         int option_index = 0;

         c = getopt_long(argc, argv, "a:g:l:r:h", long_options, &option_index);
         // long_options[option_index].name

         if (c == -1)
             break;

         switch (c){
             case 'a':
                 printf("adding UAS %s\n", optarg);
                 add_uas(root, optarg);
                 break;
             case 'g':
                 find_uas(root, optarg);
                 break;
             case 'u':
                 uas = malloc(sizeof(char) * strlen(optarg) + 1);
                 strcpy(uas, optarg);
                 break;
             case 'l':
                 add_class(root, uas, optarg);
                 break;
             case 'r':
                 break;
             case 'h':
                 print_usage();
                 exit(EXIT_SUCCESS);
                 break;
         }
     }

     if (uas != NULL) free(uas);
}

void print_usage()
{
    printf("\nua_db usage:\n\n");
    printf("ua_db add|get|add-label|remove-label|help\n");
}

void load_db(char *dbf, struct bNode *root)
{
    struct bNode *tmp = NULL;
    struct link_node_int *cls = NULL;
    struct link_node_int *cls_tmp = NULL;
    struct link_node_int *cls_last = NULL;
    char *token = NULL;
    int l = 0;
    int add = 0;
    int uas_token = 0;

    /*
     * first four bytes = num of nodes
     *
     */
    FILE *fp = fopen(dbf, "r");
    if (fp == NULL) {
        printf("Error: unable to open database file %s\n", dbf);
        exit(EXIT_FAILURE);
    }

    unsigned int numNodes = 0;
    fread(&numNodes, 4, 1, fp);

    printf("%d nodes in the database\n", numNodes);

    int c; // note: int, not char, required to handle EOF
    int i = 0;

    while ((c = fgetc(fp)) != EOF) {

        if (c == STR_SEPARATOR) {
            if (i == 0) {
                if (root->uas == NULL) {
                    bNode_set(root, token, NULL, NULL, NULL);
                    i++;
                } else {
                    tmp = bNode_create();
                    bNode_set(tmp, token, NULL, NULL, NULL);
                    bNode_add(tmp, root);
                    i++;
                }
            } else {
                // class
                if (cls == NULL) {
                    cls = link_node_int_create();
                    link_node_int_set(cls, token, 1);
                    if (root->classes == NULL) {
                        bNode_set(root, NULL, cls, NULL, NULL);
                    } else {
                        bNode_set(tmp, NULL, cls, NULL, NULL);
                    }
                } else {
                    cls_tmp = link_node_int_create();
                    link_node_int_set(cls_tmp, token, 1);
                    cls_last = link_node_int_get_last(cls);
                    cls_last->next = cls_tmp;
                }
            }
            memset(token, 0, l);
            *token = '\0';
            l = 0;
            continue;
        }

        if (c == NODE_SEPARATOR) {
            // first&last class
            if (cls == NULL) {
                cls = link_node_int_create();
                link_node_int_set(cls, token, 1);
                if (root->classes == NULL) {
                    bNode_set(root, NULL, cls, NULL, NULL);
                } else {
                    bNode_set(tmp, NULL, cls, NULL, NULL);
                }
            } else {
                cls_tmp = link_node_int_create();
                link_node_int_set(cls_tmp, token, 1);
                cls_last = link_node_int_get_last(cls);
                cls_last->next = cls_tmp;
            }
            memset(token, 0, l);
            *token = '\0';
            l = 0;
            i = 0;
            cls = NULL;
            continue;
        }

        token = realloc(token, l + 2);
        strcpy((token + l), (char *)&c);
        l += 1;
    }

    printf("\n");

    fclose(fp);
    free(token);
}

void save_db(char *dbf, struct bNode *root)
{

    FILE *fp = fopen(dbf, "w");
    if (fp == NULL) {
        printf("Error: unable to open file %s for writing\n", dbf);
        return;
    }

    if (root == NULL) {
        fclose(fp);
        printf("nothing to write to %s\n", dbf);
        return;
    }

    char *serialized = NULL;
    int len = 0;
    int num = 0;
    serialized = serialize_btree(root, serialized, &len, &num);

    /*
     * first four bytes = num of nodes
     */
    printf("\nsaving %d bytes\n", len + 4);
    fwrite(&num, 4, 1, fp);
    fwrite(serialized, sizeof(char), len, fp);

    free(serialized);

    fclose(fp);
}

char *serialize_btree(struct bNode *root, char *out, int *len, int *num)
{
    *len = 0;
    *num = 0;

    if (root == NULL) {
        printf("no root\n");
        return out;
    }

    out = serialize_bnode(root, out, len, num);

    return out;
}

char *serialize_bnode(struct bNode *node, char *out, int *len, int *num)
{
    struct link_node_int *iterator;
    char *orig = NULL;
    int l, ll;

    if (node == NULL) {
        return out;
    }

    if (node->uas != NULL) {
        printf("serializing node[%d] %s", *num, node->uas);
        *num += 1;
        l = strlen(node->uas);

        if (out != NULL) {
            orig = malloc(sizeof(char) * (*len) + 1);
            strcpy(orig, out);
        }
        *len += (l + 1);
        out = realloc(out, *len);
        if (orig != NULL) strcpy(out, orig);
        free(orig);
        strcpy((out + *len - l - 1), node->uas);
        //override the \0 char to STR_SEPARATOR
        *(out + *len - 1) = STR_SEPARATOR;
    }

    // classes
    if (node->classes != NULL) {

        iterator = node->classes;
        while(iterator) {
            if (iterator->name != NULL) {
                printf(" %s", iterator->name);
                ll = strlen(iterator->name);
                *len += (ll + 1);
                out = realloc(out, *len);
                strcpy((out + *len - ll - 1), iterator->name);
                //override the \0 char to STR_SEPARATOR
                *(out + *len - 1) = STR_SEPARATOR;
            }

            iterator = iterator->next;
        }
    }

    printf("\n");
    //override the \0 char to NODE_SEPARATOR
    if (*len > 0) *(out + *len - 1) = NODE_SEPARATOR;

    if (node->left != NULL) {
        out = serialize_bnode(node->left, out, len, num);
    }

    if (node->right != NULL) {
        out = serialize_bnode(node->right, out, len, num);
    }

    return out;
}

void add_uas(struct bNode *root, char *uas)
{
    struct bNode *new = NULL;
    struct link_node_int *classes = NULL;

    new = bNode_create();
    classes = link_node_int_create();

    link_node_int_set(classes, "<no class>", 0);
    bNode_set(new, uas, classes, NULL, NULL);

    bNode_add(new, root);
}

void find_uas(struct bNode *root, char *uas)
{
    struct bNode *tmp = NULL;

    if (root == NULL) {
        return;
    }

    if (root->uas != NULL && strcmp(root->uas, uas) == 0) {
        printf("Found %s\n", uas);
        printf("%s:", root->uas);
        print_link_node_int(root->classes);
        printf("\n");
        exit(EXIT_SUCCESS);
        return;
    }

    find_uas(root->left, uas);
    find_uas(root->right, uas);

    printf("Cannot find %s\n", uas);
    exit(EXIT_FAILURE);
}

void add_class(struct bNode *root, char *uas, char *class)
{
    struct bNode *node = bNode_get(root, uas);
    struct link_node_int *newClass = NULL;
    struct link_node_int *last = NULL;

    if (uas == NULL) {
        printf("no UAS specified!\n");
        exit(EXIT_FAILURE);
    }

    if (node == NULL) {
        printf("%s not found!", uas);
        exit(EXIT_FAILURE);
    }

    newClass = link_node_int_create();
    link_node_int_set(newClass, class, 1);
    last = link_node_int_get_last(node->classes);
    // no class yet, set the first link
    printf("val = %d\n", node->classes->val);
    if (node->classes->name != NULL && strcmp(node->classes->name, "<no class>") == 0) {
        link_node_int_set(node->classes, class, 1);
    }
    // or add at the end of the linked list
    else {
        last->next = newClass;
    }
    printf("add class %s to %s\n", class, uas);
}
