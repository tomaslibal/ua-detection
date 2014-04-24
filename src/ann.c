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

#include "ann.h"

// Module's variables
bool training_set_inited   = false;

unsigned int iter          = 0; // number of iterations in each epoch
unsigned int epochs        = 0; // number of epochs
unsigned int MAX_EPOCHS    = 10;

float t           = 0.5; // treshold
double rate       = 0.1; // learnind rate;
double weights[3] = { 0.0, 0.0, 0.0 }; // weights

// Once trained the output is saved to a data file so that the ANN doesn't
// have to train every time but only when the training set is updated or when
// the training is desired.
char training_output_filename[] = "../data/to.dat";

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
TrainingSetItem *load_training_set_from_db(unsigned int *plen)
{
    // Get the number of documents-trainingSetItems from the mongodb
    char *coll;
    coll = malloc(13);
    strcpy(coll, "training_set");
    *plen = get_doc_cnt(coll);

    static TrainingSetItem *p_internal;

    p_internal = malloc(sizeof(TrainingSetItem)*(*plen));
    if(p_internal == NULL) {
        perror("Error allocating memory");
        exit(1);
    }

    unsigned int i = 0;

    mongo *conn = dbh_get_conn();
    // query is empty because we want every document from the collection
    bson query[1];
    mongo_cursor cursor[1];

    bson_init(query);
    // empty query -eq {}
    bson_finish(query);

    mongo_cursor_init(cursor, conn, "ua_detection.training_set");
    mongo_cursor_set_query(cursor, query);

    while(mongo_cursor_next(cursor) == MONGO_OK) {
        bson_iterator input_vector[1];
        bson_iterator expected_output[1];

        if (bson_find(input_vector, mongo_cursor_bson(cursor), "input_vector") &&
        bson_find(expected_output, mongo_cursor_bson(cursor), "expected_output")) {
            printf("Expected output = %d\n", bson_iterator_int(expected_output));

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

            printf("Input Vector = %f, %f, %f\n", tmp[0], tmp[1], tmp[2] );

            p_internal[i] = (TrainingSetItem){ .input_vector = { tmp[0], tmp[1], tmp[2] }, .expected_output = bson_iterator_int(expected_output) };
        }
        i++;
    }

    bson_destroy( query );
    mongo_cursor_destroy( cursor );
    mongo_destroy(conn);

    return p_internal;
}

int train(TrainingSetItem *ts, unsigned int len)
{
    unsigned int i;
    unsigned int errCnt;
    unsigned int len_input_and_weights;
    int error;
    unsigned int j;

    int desired_output = 0;
    int result;

    double input_vector[3];
    double product;

    len_input_and_weights = sizeof(weights)/sizeof(weights[0]);

    while(true) {
        errCnt = 0;
        for(i = 0; i < len; i++) {
            iter++;

            memcpy(input_vector, ts[i].input_vector, sizeof(ts[i].input_vector));
            printf("ts[%d] is { %f, %f, %f }\n", i, ts[i].input_vector[0], ts[i].input_vector[1], ts[i].input_vector[2]);

            printf("input_vector = {%f, %f, %f}\n", input_vector[0], input_vector[1], input_vector[2]);

            memcpy(&desired_output, &ts[i].expected_output, sizeof(ts[i].expected_output));

            printf("Expected outcome = %d\n", desired_output);

            product = dot_product(input_vector, weights, sizeof(input_vector)/sizeof(input_vector[0]));
            if (product > t) {
                result = 1;
            } else {
                result = 0;
            }
            printf("Result %d\n", result);

            error  = desired_output - result;
            printf("Error %d\n", error);

            if (error != 0) {
                errCnt++;

                printf("Original weights {%f, %f, %f}\n", weights[0], weights[1], weights[2]);
                for(j = 0; j < len_input_and_weights; j++) {
                    printf("   w[j] = %f\n", rate * error * input_vector[j]);
                    weights[j] += rate * error * input_vector[j];
                }
                printf("Updated weights {%f, %f, %f}\n", weights[0], weights[1], weights[2]);
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

int parse_user_agent(char *uas, ParsedUserAgent *result)
{
    return 0;
}

int run(ParsedUserAgent *puas)
{
    return 0;
}

double avg_weights(ParsedUserAgent *puas)
{
    return 0.0;
}
double std_dev_weights(ParsedUserAgent *puas)
{
    return 0.0;
}

// Not yet implemented
// int save_training_output()
// {
//     handle = fopen(training_output_filename, "w");
//     if(handle == NULL) {
//         return 1;
//     }
//     fclose(handle);
//     return 0;
// }

// Not yet implemented
// int load_training_output()
// {
//     handle = fopen(training_output_filename, "r");
//     if(handle == NULL) {
//         return 1;
//     }
//     fclose(handle);
//     return 0;
// }
