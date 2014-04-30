#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>

#include "utils.h"
#include "ann.h"
#include "dbh.h"


/* User-Agent string to work with */
char *uas        = NULL;
/* E.g. android-devices */
char *group_name = NULL;
/* Verbose the output */
short v = 1;

// add_argument function will be used to handle the input arguments from the
// command line so that they won't have to be specified in a given order by by
// using a flag
//int add_argument(char *arg_name, char *arg_val);

int main(int argc, char *argv[])
{
    int i;

    // Get the User-Agent string and other parameters
    // If the program is started with 1 parameter only, consider that parameter
    // the user-agent string passed to the program
    if (argc > 1) {
        uas = (char*)malloc(strlen(argv[1]) + 1);
        strcpy(uas, argv[1]);
    }
    if (argc > 2) {
        group_name= (char*)malloc(strlen(argv[2]) + 1);
        strcpy(group_name, argv[2]);
    }

    if (argc == 1) {
        printf("usage: %s 'User-Agent string' 'group-id'\n", argv[0]);
        exit(1);
    }

    if(v) printf("Started with arguments, executing in cli mode\n");
    if(v) printf("Working with user-agent string %s\n", uas);
    if(v) printf("Working with group %s\n", group_name);

    if(v) printf("Testing the database handlers\n");
    dbh_test();

    // Set the group-name
    ann_set_group(group_name);

    // Load the training set and train the perceptron
    ann_training_set_t *p_ts;
    unsigned int       len;
    p_ts = load_training_set_from_db(&len);
    if (p_ts == NULL || len == 0) {
        if(v) printf("Exiting here - empty training set\n");
        return 1;
    }
    train(p_ts, len);
    free(p_ts);

    // Parse the User-Agent and save the result in the ann_parsed_user_agent struct
    ann_parsed_user_agent *puas_parsed;
    puas_parsed = malloc(sizeof(ann_parsed_user_agent));
    parse_user_agent(uas, puas_parsed);

    // Run the ann_parsed_user_agent through the perceptron
    int result = run(puas_parsed);
    if(v) printf("ANN Result: %d = %s\n", result,(result>0)?"UAS is in the GROUP":"UAS is not in the GROUP");


    // Return results in JSON format to stdout
    // ...

    free(uas);
    free(group_name);

    return 0;
}
