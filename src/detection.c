#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ann.h"

// User-Agent string to work with
char *uas;
// E.g. android-devices
char *group_name;

// Verbose the output
int v = 1;

// Used to determine if ANN is trained or not
bool ann_trained = false;

//int add_argument(char *arg_name, char *arg_val);
int free_mem();

// Reserved for future use. This function should take a variable number of
// parameters or the message will have to be formated with the values in it
// already like "value abc is set to %s", val ==> "value abc is set to xyz"
int verbose(char *message);

int main(int argc, char *argv[])
{
    int i;

    // Get the User-Agent string and other parameters
    // If the program is started with 1 parameter only, consider that parameter
    // the user-agent string passed to the program
    if (argc > 1) {
        uas = malloc(strlen(argv[1]) + 1);
        strcpy(uas, argv[1]);
        //add_argument("--ua", argv[1]);
    }
    if (argc > 2) {
        group_name= malloc(strlen(argv[2]) + 1);
        strcpy(group_name, argv[2]);
    }
    // // multiple arguments -> process the arguments one by one
    // if (argc > 2) {
    //     for(i=1;i<argc;i++) {
    //         add_argument("--ua", argv[i+1]);
    //         i++; // expects the next argmuent be the value of the prev argument name
    //     }
    // }

    // Interactive mode by default if no arguments passed
    if (argc == 1) {
        printf("Started without arguments, executing in an interactive mode\n");
        printf("Interactive mode not yet implemented. Exiting now!\n");
        exit(1);
    }

    if(v) printf("Started with arguments, executing in cli mode\n");

    // Run it through the neural network
    if(v) printf("Working with user-agent string %s\n", uas);
    if(v) printf("Working with group %s\n", group_name);

    TrainingSetItem *p_training;
    TrainingSetItem *p_ts;
    unsigned int len;
    p_training = malloc(sizeof(TrainingSetItem)*4);
    if (p_training == NULL) {
        printf("Error allocating memory for the training set");
        return 1;
    }
    //
    p_ts = load_training_set_from_db(p_training, &len);
    // if (training == NULL) {}
    //
    train(p_ts, len);
    //
    // ParsedUserAgent uas_parsed;
    // ParsedUserAgent *puas_parsed;
    // puas_parsed = &uas_parsed;
    // parse_user_agent(uas, puas_parsed);
    //
    // result = run(puas_parsed);
    // if(v) printf("ANN Result: %d", result);
    free(p_training);

    // Return results in JSON format to stdout

    free_mem();
    return 0;
}

int free_mem()
{
    free(uas);
    free(group_name);

    return 0;
}
