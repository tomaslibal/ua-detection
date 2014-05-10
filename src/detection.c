#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>

#include "utils.h"
#include "ann.h"
#include "dbh.h"
#include "uas.h"


/* User-Agent string to work with */
char *uas          = NULL;
/* E.g. android-devices */
char *group_name   = NULL;
/* Device Model */
char *device_model = NULL;
/* Verbose the output */
short v = 1;

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
        device_model = (char*)malloc(strlen(argv[2]) + 1);
        strcpy(device_model, argv[2]);
    }

    if (argc == 1) {
        printf("usage: %s 'User_agent_string' 'device_model'\n", argv[0]);
        exit(1);
    }

    if(v) printf("Working with user-agent string %s\n", uas);
    //if(v) printf("Working with device model %s\n", device_model);
    //if(v) printf("Working with group %s\n", group_name);

    if(v) printf("Testing the database handlers\n");
    dbh_test();

    // Give a list of devices that may have sent the supplied user-agent string
    // > admin:~ $ ./detection --ua 'Mozilla/5.0 (Linux; U; en-us; Android 4.4)'
    //
    // Process:
    // - Tokenize the user agent
    // - Train perceptron
    // - Test each keyword to find out if it's a product keyword
    // - Order the results
    // - Print the results
    //
    // Perceptron:
    // (input, expected) => (input: relative # of occurence, position in uas, expected: 0 or 1)

    // Load the training set and train the perceptron
    ann_training_set_t *p_ts;
    unsigned int       len;
    p_ts = load_training_set_from_db(&len, "keywords_training_set");
    if (p_ts == NULL || len == 0) {
        if(v) printf("Exiting here - empty training set\n");
        return 1;
    }
    train(p_ts, len);
    free(p_ts);

    char** k = NULL;
    int cnt = tokenize(&k, uas);
    DEBUGPRINT("Num. keywords: %d\n", cnt);

    for(int j=0;j<=cnt;j++) {
        printf(" -- Keyword[%d] = %s\n", j, k[j]);
        // init keyword vector from db
        ann_keyword ak;
        ann_keyword *pk = &ak;
        init_keyword(pk, k[j]);
        // run through the ann
        int res = run_keyword(pk);
        printf("Is product keyword = %d\n", res);
    }
    while (cnt--) free(k[cnt]);
    free(k);

    return 0;

    // Set the group-name
    //ann_set_group(group_name);

    // Set the device
    ann_set_device(device_model);



    // Parse the User-Agent and save the result in the ann_parsed_user_agent struct
    ann_parsed_user_agent parsed;
    ann_parsed_user_agent *puas_parsed;
    puas_parsed = &parsed;
    //puas_parsed = malloc(sizeof(ann_parsed_user_agent));
    parse_user_agent(uas, puas_parsed);

    // Run the ann_parsed_user_agent through the perceptron
    int result = run(puas_parsed);
    if(v) printf("ANN Result: %d", result);
    if(v && result > 0)
        printf(" FIRE!\n");
    else
        printf(" -\n");


    // Return results in JSON format to stdout
    // ...

    free(uas);
    //free(group_name);
    free(device_model);

    return 0;
}
