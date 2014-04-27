#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>

#include "ann.h"
#include "dbh.h"

// User-Agent string to work with
char *uas;
// E.g. android-devices
char *group_name;
// Verbose the output
int v = 1;
// Used to determine if ANN is trained or not
bool ann_trained = false;

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
        uas = malloc(strlen(argv[1]) + 1);
        strcpy(uas, argv[1]);
    }
    if (argc > 2) {
        group_name= malloc(strlen(argv[2]) + 1);
        strcpy(group_name, argv[2]);
    }

    // Interactive mode by default if no arguments passed
    if (argc == 1) {
        printf("Started without arguments, executing in an interactive mode\n");
        printf("Interactive mode not yet implemented. Exiting now!\n");
        exit(1);
    }

    if(v) printf("Started with arguments, executing in cli mode\n");
    if(v) printf("Working with user-agent string %s\n", uas);
    if(v) printf("Working with group %s\n", group_name);

    if(v) printf("Testing the database handlers\n");
    dbh_test();

    // Load the training set and train the perceptron
    TrainingSetItem *p_ts;
    unsigned int len;
    p_ts = load_training_set_from_db(&len);
    if (p_ts == NULL || len == 0) {
        printf("Exiting here - empty training set\n");
        return 1;
    }
    train(p_ts, len);

    // Parse the User-Agent and save the result in the ParsedUserAgent struct
    static ParsedUserAgent uas_parsed;
    ParsedUserAgent *puas_parsed;
    puas_parsed = &uas_parsed;
    parse_user_agent(uas, puas_parsed);

    // Run the ParsedUserAgent through the perceptron
    int result = run(puas_parsed);
    if(v) printf("ANN Result: %d = %s\n", result,(result>0)?"UAS is in the GRUP":"UAS is not in the GROUP");
    free(p_ts);

    // Return results in JSON format to stdout
    // ...

    free(uas);
    free(group_name);

    return 0;
}
