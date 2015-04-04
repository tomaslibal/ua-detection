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

/*
 * Database file for the user agent strings
 */
char *dbf = "data/ua.db";

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
    char *tmp = NULL;
    char *uas = NULL;

    static struct option long_options[] = {
            { "add", required_argument, 0, 'a' },
            { "get", required_argument, 0, 'g' },
            { "update", required_argument, 0, 'u' },
            { "delete", required_argument, 0, 'd' },
            { "add-class", required_argument, 0, 'l' },
            { "remove-class", required_argument, 0, 'r' },
            { "help", no_argument, 0, 'h' }
     };

     while (1) {
         int option_index = 0;

         c = getopt_long(argc, argv, "a:g:u:d:l:r:h", long_options, &option_index);
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
             case 'd':
                 delete_uas(root, optarg);
                 break;
             case 'l':
                 add_class(root, uas, optarg);
                 break;
             case 'r':
                 remove_class(root, uas, optarg);
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
