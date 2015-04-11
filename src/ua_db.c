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

// http://userpages.umbc.edu/~mabzug1/cs/md5/md5.html
// http://sourceforge.net/projects/libmd5-rfc/files/

#include "db.h"

void print_usage();
void read_cli_arguments(int argc, char **argv);
void generate_clf_set(char *class, struct bNode *root);
void writeout_btree(struct bNode *root, FILE *fp, char *class);
void write_line_to_file(FILE *fp, char *line);

/*
 * Database file for the user agent strings
 */
char *dbf = "data/ua.db";

/*
 * Training set file
 */
char *out = "out.txt";

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
     *     ua_db --add "Mozilla/5.0 (Linux; Android) Firefox/35.0"
     *     ua_db --get "Mozilla/5.0 ..."
     *     ua_db --update <uas> --add-class <label>
     *     ua_db --update <uas> --remove-class <label>
     *
     *     @todo implement UAS-ID so that the update/get operations won't
     *     need the whole <uas> string but could take an ID instead
     */

    root = bNode_create();

    load_db(dbf, root);

    read_cli_arguments(argc, argv);

    save_db(dbf, root);

    bNode_free(root);

    return 0;
}

void read_cli_arguments(int argc, char **argv)
{
    int c;
    int use_id = 0;
    int res;
    char *tmp = NULL;
    char *uas = NULL;

    static struct option long_options[] = {
            { "by-id", no_argument, 0, 'i' },
            { "add", required_argument, 0, 'a' },
            { "get", required_argument, 0, 'g' },
            { "update", required_argument, 0, 'u' },
            { "delete", required_argument, 0, 'd' },
            { "add-class", required_argument, 0, 'l' },
            { "remove-class", required_argument, 0, 'r' },
            /*
             * UADB can generate a text file which is supposed to be a training
             * set for a binary classifier. Each line is a pair of "class, UAS"
             * where class is specified by the optarg of 'generate-cls' option.
             *
             * For instance, if 'generate-cls mobile' is supplied, the output
             * file will have pairs "mobile, UAS" and "all-other, UAS" for
             * user-agent strings that have 'mobile' class and those who do
             * not, respectivelly.
             */
            { "generate-cls", required_argument, 0, 'o' },
            { "help", no_argument, 0, 'h' }
     };

     while (1) {
         int option_index = 0;

         c = getopt_long(argc, argv, "ia:g:u:d:l:r:o:h", long_options, &option_index);
         // long_options[option_index].name

         if (c == -1)
             break;

         switch (c){
             case 'i':
                 use_id = 1;
                 break;
             case 'a':
                 printf("adding UAS %s\n", optarg);
                 add_uas(root, optarg);
                 break;
             case 'g':
                 if (use_id == 1) {
                     res = find_by_id(root, atoi(optarg));
                     if (!res) {
                         printf("Cannot find a record with id=%d\n", atoi(optarg));
                     }
                 } else {
                     res = find_uas(root, optarg);
                     if (!res) {
                         printf("Cannot find a record with uas=%s\n", optarg);
                     }
                 }
                 break;
             case 'u':
                 uas = malloc(sizeof(char) * strlen(optarg) + 1);
                 strcpy(uas, optarg);
                 break;
             case 'd':
                 delete_uas(root, optarg);
                 break;
             case 'l':
                 add_class(root, uas, optarg);
                 break;
             case 'r':
                 remove_class(root, uas, optarg);
                 break;
             case 'o':
                 generate_clf_set(optarg, root);
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
    printf("\tby-id                if -i/--by-id used, instead of lookup by user-agent\n\t                     string the ID is used to find records\n");
    printf("\tadd <uas>            adds a record\n");
    printf("\tget <uas/id>         retrieve the matching record\n");
    printf("\tupdate <uas/id>      use for update operations like add-class or\n\t                     remove-class\n");
    printf("\tdelete <uas/id>      removes the matching record\n");
    printf("\tadd-class <class>    adds a class, must be used in conjunction\n\t                     with --update <uas/id>\n");
    printf("\tremove-class <class> removes a class, must be used in conjunction\n\t                     with --update <uas/id>\n");
    printf("\tgenerate-cls <class> saves a file\n");
    printf("\thelp                 displays this help\n");
}

/*
 * Generate a training set for a given class. The strategy is one-vs-all.
 *
 * Each line in the output file will have a string of text (ascii) which
 * is a tuple <class UAS> (separated by the first whitespace char).
 *
 */
void generate_clf_set(char *class, struct bNode *root)
{
    FILE *fp = NULL;

    fp = fopen(out, "w");

    if (fp == NULL) {
        perror("cannot write the file\n");
        return;
    }

    writeout_btree(root, fp, class);

    fclose(fp);
}

void writeout_btree(struct bNode *root, FILE *fp, char *class)
{
    struct link_node_int *tmp;
    char other[32] = "other";
    char *line = NULL;
    int offset = 0;
    char *lf = NULL;

    if (root == NULL) {
        return;
    }

    if (root->uas != NULL) {
        tmp = link_node_int_get(root->classes, class);
        lf = NULL;
        if (tmp != NULL) {
            line = malloc(sizeof(char) * strlen(class) + sizeof(char) * strlen(root->uas) + 3); // +3 = 1 for null terminator, one for the space char between class and UAS and one for the new line char
            memset(line, 0, sizeof(line));
            strcpy(line, class);
            line[strlen(class)] = ' ';
            strcpy((line+strlen(class)+1), root->uas);
            lf = strchr(line, '\n');
            if (!lf)
                line[strlen(line)] = '\n';

            lf = strstr(line, "\n\n");
            if (lf) {
                lf[1] = '\0';
            }

            write_line_to_file(fp, line);
        } else {
            line = malloc(sizeof(char) * strlen("other ") + sizeof(char) * strlen(root->uas) + 2);
            memset(line, 0, sizeof(line));
            strcpy(line, "other ");
            strcpy((line+strlen("other ")), root->uas);

            lf = strchr(line, '\n');

            if (lf == NULL) {
                line[strlen(line)] = '\n';
            }

            lf = strstr(line, "\n\n");
            if (lf) {
                lf[1] = '\0';
            }

            write_line_to_file(fp, line);
        }
        free(line);
        line = NULL;
    }

    writeout_btree(root->left, fp, class);
    writeout_btree(root->right, fp, class);
}

void write_line_to_file(FILE *fp, char *line)
{
    int ret = fwrite(line, sizeof(char), strlen(line), fp);
}
