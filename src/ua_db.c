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

void read_cli_arguments(int argc, char **argv);
void print_usage();

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

    read_cli_arguments(argc, argv);

    return 0;
}

void read_cli_arguments(int argc, char **argv)
{
    int c;

    static struct option long_options[] = {
            { "add", required_argument, 0, 'a' },
            { "get", required_argument, 0, 'g' },
            { "add-label", required_argument, 0, 'l' },
            { "remove-label", required_argument, 0, 'r' },
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
                 break;
             case 'g':
                  break;
             case 'l':
                  break;
             case 'r':
                  break;
             case 'h':
                 print_usage();
                 exit(0);
                 break;
         }
     }
}

void print_usage()
{
    printf("\nua_db usage:\n\n");
    printf("ua_db add|get|add-label|remove-label|help\n");
}
