//
// Artificial Neural Network
// A          N      N
//
// So far this is just a single perceptron
// But depending on the need or if needed to achieve better results it may
// change into a multi-layer neural network with eventual other features to
// get better results.
//
// ann.c

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <math.h>

#include "ann.h"

unsigned int iter          = 0; // number of iterations in each epoch
unsigned int epochs        = 0; // number of epochs
unsigned int MAX_EPOCHS    = 100;

float t           = 0.5; // treshold
double rate       = 0.1; // learnind rate;
double weights[3] = { 0.0, 0.0, 0.0 }; // weights

// If the group name is specified during the query, this variable will be populated
// with a 24 character ObjectId of that group, or NULL if unitialized or on error
char *uas_device_group_id = NULL;
char *uas_device_group_name;
char *uas_device_id;
char *uas_device_model;

// Calculates the dot product of two vectors
// Precondition:
//     values: input vector (e.g. (1, 0, 0)),
//     weights: current weights in an iteration (e.g. (0.9, 0.7, -0.1))
//              must match the size of the input vector
//     lenghts of values and weights must be exactly the same
// Postcondition:
//     k1*l1 + k2*l2 + ... + kn*ln
double dot_product(double *values, double *weights, unsigned int len)
{
    int    i;
    double sum = 0;

    // Where should the precondition checking be included, if at all?
    // we can't make the assertion here because we only have the pointers
    // assert(sizeof(values) == sizeof(weights))

    for(i = 0; i < len; i++) {
        sum += values[i] * weights[i];
    }

    return sum;
}

// This function loads the training set from the database
// and keeps it in the memory
ann_training_set_t *load_training_set_from_db(unsigned int *plen, const char *collection)
{
    //char         *coll = NULL;
    //coll               = (char*)malloc(13);
    //if(coll == NULL){ DEBUGPRINT("Malloc error in load_training_set_from_db()\n"); exit(1); }
    //strcpy(coll, "training_set");
    *plen              = get_doc_cnt(collection);
    unsigned int i     = 0;
    mongo        *conn = dbh_get_conn();
    bson         query[1];
    mongo_cursor cursor[1];

    static ann_training_set_t *p_internal = NULL;

    // If the training set collection is empty, return
    DEBUGPRINT("Length of the training set = %d\n", *plen);
    if (*plen == 0) {
        printf("*** WARNING *** Training set empty!\n");
        return p_internal;
    }

    p_internal = (ann_training_set_t*)calloc(*plen, sizeof(ann_training_set_t));
    if(p_internal == NULL) {
        DEBUGPRINT("Error allocating memory\n");
        exit(1);
    }

    bson_init(query);
    // empty query -eq {}
    // query is empty because we want every document from the collection
    bson_finish(query);

    char *db_and_coll = malloc(100);
    strcpy(db_and_coll, "ua_detection.");
    strcpy(db_and_coll+13, collection);

    mongo_cursor_init(cursor, conn, db_and_coll);
    free(db_and_coll);
    mongo_cursor_set_query(cursor, query);

    while(mongo_cursor_next(cursor) == MONGO_OK) {
        bson_iterator input_vector[1];
        bson_iterator expected_output[1];

        if (bson_find(input_vector, mongo_cursor_bson(cursor), "input_vector") &&
        bson_find(expected_output, mongo_cursor_bson(cursor), "expected_output")) {

            bson_iterator sub[1];

            double tmp[3];
            unsigned int j = 0;
            bson_iterator_subiterator(input_vector, sub);

            // init the input_vector values one by one
            while(bson_iterator_more(sub)) {
                if(bson_iterator_next(sub)!=BSON_EOO) {
                    tmp[j++] = bson_iterator_double(sub);
                }
            }

            DEBUGPRINT("Input Vector = %f, %f, %f\n", tmp[0], tmp[1], tmp[2] );

            p_internal[i] = (ann_training_set_t){ .input_vector = { tmp[0], tmp[1], tmp[2] }, .expected_output = bson_iterator_int(expected_output) };
        }
        i++;
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );
    mongo_destroy(conn);

    return p_internal;
}

int train(ann_training_set_t *ts, unsigned int len)
{
    unsigned int i;
    unsigned int errCnt;
    unsigned int len_input_and_weights;
    int          error;
    unsigned int j;
    int          desired_output = 0;
    int          result;
    double       input_vector[3];
    double       product;

    len_input_and_weights = sizeof(weights)/sizeof(weights[0]);

    while(true) {
        errCnt = 0;
        for(i = 0; i < len; i++) {
            iter++;

            memcpy(input_vector, ts[i].input_vector, sizeof(ts[i].input_vector));
            DEBUGPRINT("ts[%d] is { %f, %f, %f }\n", i, ts[i].input_vector[0], ts[i].input_vector[1], ts[i].input_vector[2]);

            DEBUGPRINT("input_vector = {%f, %f, %f}\n", input_vector[0], input_vector[1], input_vector[2]);

            memcpy(&desired_output, &ts[i].expected_output, sizeof(ts[i].expected_output));

            DEBUGPRINT("Expected outcome = %d\n", desired_output);

            product = dot_product(input_vector, weights, sizeof(input_vector)/sizeof(input_vector[0]));
            if (product > t) {
                result = 1;
            } else {
                result = 0;
            }
            DEBUGPRINT("Result %d\n", result);

            error  = desired_output - result;
            DEBUGPRINT("Error %d\n", error);

            if (error != 0) {
                errCnt++;

                DEBUGPRINT("Original weights {%f, %f, %f}\n", weights[0], weights[1], weights[2]);
                for(j = 0; j < len_input_and_weights; j++) {
                    DEBUGPRINT("   updating w[%d] to %f\n", j, rate * error * input_vector[j]);
                    weights[j] += rate * error * input_vector[j];
                }
                DEBUGPRINT("Updated weights {%f, %f, %f}\n", weights[0], weights[1], weights[2]);
            }
        }
        if (errCnt == 0) {
            break;
        }

        if(epochs++ > MAX_EPOCHS) {
            printf("Terminating the loop! MAX_EPOCHS reached");
            break;
        }
    }
    printf("Training done. Weights %f %f %f\n", weights[0], weights[1], weights[2]);

    return 0;
}

int ann_set_device(const char *model)
{
    //
    uas_device_id = (char*)malloc(25);
    // hardcoded Device ID for testing now...
    strcpy(uas_device_id, "535ab70328328433d64c3d7d");
    uas_device_model = (char*)malloc(strlen(model)+1);
    strcpy(uas_device_model, model);
    return 0;
}

int ann_set_group(const char *name)
{
    if(name == NULL) {
        uas_device_group_name = malloc(2);
        strcpy(uas_device_group_name, "");
        return 1;
    }

    mongo *conn         = dbh_get_conn();
    uas_device_group_id = (char*)malloc(25);
    if(uas_device_group_id == NULL){perror("Malloc error\n");exit(1);}

    uas_device_group_name = malloc(sizeof(char)*strlen(name) + 1);
    strcpy(uas_device_group_name, name);

    if(name == NULL){ DEBUGPRINT("Group not set\n"); return 1;}
    // Look Up Group ID
    printf("Looking up group ID for group %s\n", name);
    bson         query[1];
    mongo_cursor cursor[1];

    bson_init(query);
        bson_append_string(query, "short_name", name);
    bson_finish(query);

    mongo_cursor_init(cursor, conn, "ua_detection.groups");
    mongo_cursor_set_query(cursor, query);
    int j = 0;
    while(mongo_cursor_next(cursor) == MONGO_OK) {
        bson_iterator id[1];
        bson_oid_t    oid[1];
        char          *oid_str = NULL;
        DEBUGPRINT("mongo_ok\n");
        if (bson_find(id, mongo_cursor_bson(cursor), "_id")) {
            oid_str = (char*)malloc(25);
            if(oid_str == NULL){ perror("Malloc error");exit(1); }
            bson_oid_to_string(bson_iterator_oid(id), oid_str);
            DEBUGPRINT("Group's ID = %s\n", oid_str);
            strcpy(uas_device_group_id, oid_str);
        }else {
            DEBUGPRINT("Group ID not found\n");
        }
        j++;
    }

    if(j==0) {
        DEBUGPRINT("Error while retrieving the cursor\n");
        strcpy(uas_device_group_id, "535ab67328328433d64c3d7b");
        uas_device_group_name = realloc(uas_device_group_name, sizeof(char)*8);
        strcpy(uas_device_group_name, "Default");
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );

    mongo_destroy(conn);
    return 0;
}

int parse_user_agent(char *uas, ann_parsed_user_agent *result)
{
    char   *kws[50];
    double w[50];
    memset(w, 0.0, 50);
    int    len  = 0;

    len = split_keywords(uas, kws);
    DEBUGPRINT("No. of keywords = %d\n", len);
    get_weights(kws, len, w);

    memcpy(result->keywords, kws, sizeof(*kws)*len);
    memcpy(result->weights, w, sizeof(*w)*len);
    result->cnt = len;
    result->char_cnt = strlen(uas);

    //free(uas_device_group_id);

    return 0;
}

// Matches a regex against a string
// regex_t *re pointer to a regex_t regular expression
// char *substr is the subject string
// char *ptr[] array of pointers to the individual matches
// returns unsigned int length of the ptr[] array
unsigned int match_regex(regex_t *re, char *substr, char *ptr[])
{
    // Will point to the previous match
    char *p             = substr;
    const int n_matches = 30;
    unsigned int j      = 0;

    // Matches kept in this array
    regmatch_t rm[n_matches];

    while(1) {
        int i = 0;
        int no_match = regexec(re, p, n_matches, rm, 0);
        if (no_match) {
            return j;
        }

        for(i = 0; i < n_matches; i++) {
            int start;
            int end;

            if (rm[i].rm_so == -1)
                break;

            start = rm[i].rm_so + (p - substr);
            end = rm[i].rm_eo + (p - substr);

            ptr[j] = (char*)malloc(sizeof(char)*(end-start) + 1);
            if(ptr[j] == NULL){ perror("Malloc error\n"); exit(1); }
            sprintf(ptr[j], "'%.*s'", (end-start), substr + start);
            j++;
        }
        p += rm[0].rm_eo;
    }
    return 0;
}

int split_keywords(char *uas, char **arr)
{
    regex_t re;
    regmatch_t rm;
    int status;
    char msgbuf[100];

    // Compile the regular expression
    // /([\w.]+(|\/)[0-9.]+|[\w.]+)/ -- PCRE regex used in node.js tools
    // This package <regex.h> works with POSIX regex
    if (regcomp(&re, "[a-zA-Z0-9.]+/[0-9.]+|[a-zA-Z0-9.]+", REG_EXTENDED|REG_ICASE) != 0) {
        return 0;
    }

    // Results kept in this array
    char *r[30];
    //memset(r, 0, 30);
    // Number of found keywords
    int no;

    no = match_regex(
        &re,
        uas,
        r
    );

    printf("size of arr = %lu\n", sizeof(arr));

    for(int i = 0; i < no; i++) {
        if(r[i] != NULL) {
            if (NULL == (arr[i] = malloc(strlen(r[i])+1))) {
                printf("Memory allocation error. Exiting");
                return 1;
            }
            strcpy(arr[i], r[i]);
            remove_quotes(arr[i]);

            DEBUGPRINT("[split_keywords] Keyword[%d] addr = %p\n", i, &arr[i]);
            DEBUGPRINT("[split_keywords] Keyword[%d] = %s\n", i, arr[i]);
        }
    }

    regfree(&re);
    return no;
}

// Given an array of keywords e.g. ["Mozilla/5.0", "Linux", "U", ..., "Android"]
// this function fetches the corresponding weight value from MongoDB for each
// keyword and stores the weights in an array.
//
// char **keywords pointer to a mem location which is an array of pointers of
// the individual keywords
// int cnt is the number of keywords
// double **w pointer to a mem location which is an array of pointers to the
// individual weights
// char *group_id is the ID of the group for which the program lookups up if the
// user agent string belongs to that group or not
// returns 0 on success
int get_weights(char **keywords, int cnt, double *w)
{
    short i = 0;
    mongo *conn = dbh_get_conn();

    for(i = 0; i < cnt; i++) {
        DEBUGPRINT("[get_weights] Keyword[%d] addr = %p\n", i, &keywords[i]);
        DEBUGPRINT("[get_weights] Keyword[%d] = %s\n", i, keywords[i]);
        DEBUGPRINT("[get_weights] strlen(keyword[%d]) = %lu\n", i, strlen(keywords[i]));

        bson query[1];
        mongo_cursor cursor[1];

        DEBUGPRINT("[get_weights] device_id = %s\n", uas_device_id);
        DEBUGPRINT("[get_weights] device_model = %s\n", uas_device_model);

        remove_quotes(uas_device_model);

        bson_init( query );
            bson_append_string( query, "value", keywords[i]);
        bson_finish( query );

        mongo_cursor_init( cursor, conn, "ua_detection.keywords" );
        mongo_cursor_set_query( cursor, query );

        int j = 0;
        while( mongo_cursor_next( cursor ) == MONGO_OK ) {
            bson_iterator devices_weights[1];
            bson_iterator sub[1];

            if ( bson_find(devices_weights, mongo_cursor_bson(cursor), "devices_weights") ) {
                bson_iterator_subiterator(devices_weights, sub);
                while(bson_iterator_more(sub)) {
                    if (bson_iterator_next(sub) != BSON_EOO) {
                        if(strcmp(bson_iterator_key(sub), uas_device_id) == 0) {
                            w[i] = bson_iterator_double(sub);
                            j++;
                            printf("Keyword %s: weight = %f\n", keywords[i], w[i]);
                        }
                    }
                }
            }
        }

        if(j==0) {
            DEBUGPRINT("[get_weights] Nothing found!\n");
        }

        bson_destroy( query );
        mongo_cursor_destroy( cursor );
    }

    mongo_destroy(conn);
    return 0;
}

int run(ann_parsed_user_agent *puas)
{
    if(puas == NULL) {
        printf("No Parsed User-Agent. Exiting.\n"); exit(1);
    }

    // calculate the input vector
    double const avg = sample_mean(puas->weights, puas->cnt);
    double const std_dev = std_dev_weights(puas, avg);
    double const len = (double)puas->char_cnt;

    double vector[3] = { avg, std_dev, 1.0 };

    double dp = dot_product(vector, weights, 3);
    DEBUGPRINT("resulting dot product = %f\n", dp);
    DEBUGPRINT("avg = %f; std_dev = %f, len = %f\n", avg, std_dev, len);
    return (dp > t) ? 1 : 0;
}

double std_dev_weights(ann_parsed_user_agent *puas, double avg)
{
    int i = 0;
    double sum = 0;
    for(;i<puas->cnt;i++){
        sum += pow((puas->weights[i] - avg), 2);
    }
    return sqrt(sum/(i-1));
}

void remove_quotes(char* str)
{
    char *dst = str;
    char *src = str;
    char c;

    while ((c = *src++) != '\0')
    {
        if (c != '\'')
            *dst++ = c;
    }
    *dst = '\0';
}
