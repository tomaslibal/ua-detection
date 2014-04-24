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

// Used to parse the User-Agent strings into individual keywords
unsigned int match_regex(regex_t *re, const char *substr, char *ptr[]);

//int add_argument(char *arg_name, char *arg_val);
int free_mem();

// Reserved for future use. This function should take a variable number of
// parameters or the message will have to be formated with the values in it
// already like "value abc is set to %s", val ==> "value abc is set to xyz"
int verbose(char *message);

int split_keywords(char *uas, unsigned int len);

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

    // Run it through the neural network
    if(v) printf("Working with user-agent string %s\n", uas);
    if(v) printf("Working with group %s\n", group_name);

    if(v) printf("Testing the database handlers\n");
    dbh_test();

    TrainingSetItem *p_ts;
    unsigned int len;

    p_ts = load_training_set_from_db(&len);
    //return 0;
    train(p_ts, len);

    unsigned int uas_len = 0;
    split_keywords(uas, uas_len);
    //
    // ParsedUserAgent uas_parsed;
    // ParsedUserAgent *puas_parsed;
    // puas_parsed = &uas_parsed;
    // parse_user_agent(uas, puas_parsed);
    //
    // result = run(puas_parsed);
    // if(v) printf("ANN Result: %d", result);
    free(p_ts);

    // Return results in JSON format to stdout

    free_mem();
    return 0;
}

unsigned int match_regex(regex_t *re, const char *substr, char *ptr[])
{
    const char *p       = substr;
    const int n_matches = 20;
    unsigned int j      = 0;

    // Matches kept in this array
    regmatch_t rm[n_matches];
    // Clean results in an array of strings
    char *results[n_matches];

    while(1) {
        int i = 0;
        int no_match = regexec(re, p, n_matches, rm, 0);
        if (no_match) {
            memcpy(ptr, results, sizeof(results));
            return j;
        }

        for(i = 0; i < n_matches; i++) {
            int start;
            int end;

            if (rm[i].rm_so == -1)
                break;

            start = rm[i].rm_so + (p - substr);
            end = rm[i].rm_eo + (p - substr);

            char *tmp = malloc(sizeof(char)*strlen(substr));
            strncpy ( tmp, substr + start, end - start);
            results[j] = tmp;
        }
        j++;
        p += rm[0].rm_eo;
    }
    return 0;
}

int split_keywords(char *uas, unsigned int len)
{
    regex_t re;
    regmatch_t rm;
    int status;
    char msgbuf[100];

    // Compile the regular expression
    // /([\w.]+(|\/)[0-9.]+|[\w.]+)/
    if (regcomp(&re, "[a-zA-Z0-9.]+/[0-9.]+|[a-zA-Z0-9.]+", REG_EXTENDED|REG_ICASE) != 0) {
        return 0;
    }

    // Results kept in this array
    char *r[20];
    // Number of found keywords
    unsigned int no;

    no = match_regex(
        &re,
        uas,
        r
    );

    for(int i = 0; i < no; i++) {
        if(r[i] != NULL) {
            printf("%s\n", r[i]);
        }
    }

    regfree(&re);
    return 0;
}

int free_mem()
{
    free(uas);
    free(group_name);

    return 0;
}
