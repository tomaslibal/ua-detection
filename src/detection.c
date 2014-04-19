#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "ann.h"

// User-Agent string to work with
char *uas;
// E.g. android-devices
char *group_name;

// Verbose the output
int v = 1;

// Used to determine if ANN is trained or not
bool ann_trained = false;

int add_argument(char *arg_name, char *arg_val);

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
    if (argc == 2) {
        add_argument("--ua", argv[1]);
    }
    // multiple arguments -> process the arguments one by one
    if (argc > 2) {
        for(i=1;i<argc;i++) {
            add_argument("--ua", argv[i+1]);
            i++; // expects the next argmuent be the value of the prev argument name
        }
    }

    // Interactive mode by default if no arguments passed
    if (argc == 1) {
        printf("Started without arguments, executing in an interactive mode\n");
        printf("Interactive mode not yet implemented. Exiting now!\n");
        exit(1);
    }

    if(v) printf("Started with arguments, executing in cli mode\n");

    // Run it through the neural network
    if(v) printf("Working with user-agent string %s\n", uas);

    // TrainingSetItem *training;
    // unsigned int trainingSetLength;
    //
    // load_training_set_from_db(training, &trainingSetLength);
    // if (training == NULL) {}
    //
    // train(training, trainingSetLength);
    //
    // ParsedUserAgent uas_parsed;
    // ParsedUserAgent *puas_parsed;
    // puas_parsed = &uas_parsed;
    // parse_user_agent(uas, puas_parsed);
    //
    // result = run(puas_parsed);
    // if(v) printf("ANN Result: %d", result);
    // free(training);

    // Return results in JSON format to stdout

    free_mem();
    return 0;
}

int add_argument(char *arg_name, char *arg_val)
{
    size_t len = strlen(arg_val) + 1;
    char *buffer = malloc(len); if(buffer == NULL) { exit(1); }
    strcpy(buffer, arg_val);

    // User-Agent string setter
    if(strcmp(arg_name, "--ua") == 0) {
        uas = malloc(len); if(uas == NULL) { exit(1); }
        memcpy(uas, buffer, len);
        if(v) printf("Adding: %s as %s\n", arg_name, arg_val);
    // Group (name) setter
    } else if (strcmp(arg_name, "--group")) {
        group_name = malloc(len); if(group_name == NULL) { exit(1); }
        memcpy(group_name, buffer, len);
        if(v) printf("Adding: %s as %s\n", arg_name, arg_val);
    } else {
        printf("Unknown argument %s\n", arg_name);
    }

    free(buffer);
    return 0;
}

int free_mem()
{
    free(uas);
    free(group_name);

    return 0;
}
