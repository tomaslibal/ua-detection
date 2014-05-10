#ifndef ANN_HEADER
#define ANN_HEADER

#include "utils.h"
#include "dbh.h"

// Training Set's struct
//
// Has the signature of
//   {
//     input_vector = { 0.0, 0.0, 0.0 }
//     expected_output = 0
//   }
//
// Expected_output should be values 0 or 1: value 0 should be when the neuron is not expected to fire with the given
// Input_vector and value 1 for situations when the neuron is expected to fire with the given input_vector.
//
// Tentatively:
//   input_vector = { avg_weight, std_dev_weight, char_count }
//
//
typedef struct
{
    double input_vector[3];
    int expected_output;
} ann_training_set_t;

// When in the production mode (i.e. trained) the user-agent strings
// will be passed to the run() function.
//
// ! IMPORTANT ! For now we assume that the program will only calculate
// the possibility of the user-agent string being from a phone of a particular
// group. I.e. Is "Linux (Android 4.4; Xperia Z1)" from "Android Phones"?
//
// This string will have to be processed:
//
//     1) decouple the keywords
//     2) lookup every keyword in the database and find its weight for the given group
//     3) calculate:
//        3a) Average of weights
//        3b) Standard deviation of the weights
//        3c) Character count of the user-agent string
//     4) Allocate a 'queryItem' with the 3 values from above as input_vector
//     5) Run the queryItem through the perceptron's weights
//     Return the result (1 or 0)
typedef struct
{
    double input_vector[3];
} ann_query_item;

typedef struct
{
    char *keywords[50];
    double weights[50];
    int cnt;         // length of the keywords and weights arrays
    int char_cnt;    // character count of all keywords together
} ann_parsed_user_agent;

typedef struct
{
    char *keyword;
    double cst; // constant
    double occ; // occurence
    double pos; // position
} ann_keyword;


ann_training_set_t *load_training_set_from_db(unsigned int *plen, const char *collection);
/* calculates a vector dot product of two given vectors */
double dot_product(double *values, double *weights, unsigned int len);
/* calculates sample mean of the weights of a parsed user-agent */
double avg_weights(ann_parsed_user_agent *puas);
/* calculates sample standard deviation of the weights of a parsed user-agent */
double std_dev_weights(ann_parsed_user_agent *puas, double avg);
int train(ann_training_set_t *ts, unsigned int len);
int parse_user_agent(char *uas, ann_parsed_user_agent *result);
int ann_set_group(const char *name);
int ann_set_device(const char *model);
int split_keywords(char *uas, char **arr);
/* get weights for corresponding keywords from the database */
int get_weights(char **keywords, int cnt, double *w);
int run(ann_parsed_user_agent *puas);
int run_keyword(ann_keyword *pk);
unsigned int match_regex(regex_t *re, char *substr, char **ptr);
void remove_quotes(char* str);

#endif
