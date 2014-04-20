#ifndef ANN_HEADER
#define ANN_HEADER

// Module's variables
bool training_set_inited   = false;

unsigned int iter          = 0; // number of iterations in each epoch
unsigned int epochs        = 0; // number of epochs
unsigned int MAX_EPOCHS    = 999999;

float t           = 0.5; // treshold
double rate       = 0.1; // learnind rate;
double weights[3] = { 0.0, 0.0, 0.0 }; // weights

//trainingSetItem trainingSet[];

// Once trained the output is saved to a data file so that the ANN doesn't
// have to train every time but only when the training set is updated or when
// the training is desired.
char training_output_filename[] = "../data/to.dat";

// Training Set's struct
//
// Has the signature of
// {
//   input_vector = { 0.0, 0.0, 0.0 }
//   expected_output = 0
// }
//
// expected_output is expected to be values 0 or 1 only
// Value 0 should be when the neuron is not expected to fire with the given
// input_vector and value 1 for situations when the neuron is expected to
// fire with the given input_vector.
//
// Tentatively:
// input_vector = { avg_weight, std_dev_weight, str_count }
//
// Memory footprint double(64 bits) * 3 + int(32 bits) = 224 bits
//
typedef struct
{
    double input_vector[3];
    int expected_output;
} TrainingSetItem;

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
} QueryItem;

// Parsed User-Agent String
//
typedef struct
{
    char *keywords;
    double *weights;
    int cnt; // length of the keywords and weights arrays
    int char_count; // character count of all keywords together
} ParsedUserAgent;

// Functions' prototypes
// UNSTABLE - WILL PROBABLY CHANGE
double dot_product(double *values, double *weights, unsigned int len);
double avg_weights(ParsedUserAgent *puas);
double std_dev_weights(ParsedUserAgent *puas);

int load_training_set_from_db(TrainingSetItem *ts, int *plen);
int train(TrainingSetItem *ts, int len);
int parse_user_agent(char *uas, ParsedUserAgent *result);
int run(ParsedUserAgent *puas);

// For later implementation:
int save_training_output();
int load_training_output();

#endif
