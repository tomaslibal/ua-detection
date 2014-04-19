#include <string.h>
#include <bool.h>
#include "ann.h"

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

// Calculates the dot product of two vectors
// Precondition:
//     values: input vector (e.g. (1, 0, 0)),
//     weights: current weights in an iteration (e.g. (0.9, 0.7, -0.1))
//              must match the size of the input vector
// Postcondition:
//     k1*l1 + k2*l2 + ... + kn*ln
double dot_product(double *values, double *weights)
{
    int    i   = 0;
    int    len = sizeof(values)/sizeof(values[0]);
    double sum = 0;

    // Where should the precondition checking be included, if at all?
    // assert(sizeof(values) == sizeof(weights))

    for(i; i < len; i++) {
        sum += values[i] * weights[i];
    }

    return sum;
}

// This function loads the training set from the database
// and keeps it in the memory
int load_training_set_from_db(TrainingSetItem *ts, int *plen)
{
    // Get the number of documents-trainingSetItems from the mongodb
    unsigned int cnt;
    cnt = 10; // mock value
    *trainingSetLength = &cnt;
    // Allocate memory for the training set
    training = malloc(sizeof(trainingSetItem) * cnt);
    if (training == NULL) {
        printf("Error allocating memory for the training set");
        return 1;
    }
    // Initialize the training set
    // ...
    
    // Done
    return 0;
}

int train(TrainingSetItem *ts, int len)
{
    unsigned int i;
    unsigned int errCnt;
    int err;

    while(true) {
        if(epochs++ > MAX_EPOCHS) {
            printf("Terminating the loop! MAX_EPOCHS reached")
            break;
        }

        for(i = 0; i < len; i++) {
            iteration++;

            input_vector = trainingSet[i].input_vector;
            desired_output = trainingSet[i].expected_output;

            result = dot_product(input_vector, weights) > t;
            error  = desired_output - result;

            if (error != 0) {
                errCnt++;

                for(j = 0; j < sizeof(weights)/sizeof(weights[0]); j++) {
                    weights[j] += rate * error * weights[j];
                }
            }
        }
        if (errCnt == 0) {
            break;
        }
    }
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

double dot_product(double *values, double *weights)
{
    return 0.0;
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
