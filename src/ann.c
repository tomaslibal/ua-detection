#include <string.h>
#include "ann.h"

// Artificial Neural Network

int iter          = 0;
float t           = 0.5; // treshold
double rate       = 0.1; // learnind rate;
double weights[3] = { 0.0, 0.0, 0.0 }; // weights

// Once trained the output is saved to a data file so that the ANN doesn't
// have to train every time but only when the training set is updated or when
// the training is desired.
char training_output_filename[] = "tro.dat";

// Function prototypes should be in ann.h file
// Here will be the  functions' implementations

// values: input vector (e.g. (1, 0, 0)),
// weights: current weights in an iteration (e.g. (0.9, 0.7, -0.1))
//          must match the size of the input vector 
double dot_product(double *values, double *weights) 
{
  int    i   = 0;
  double sum = 0;

  for(i; i < sizeof(values); i++) {
    sum += values[i] * weights[i];
  }

  return sum;
}

int load_training_set();
int save_training_output();
int load_training_output();
int train();
int run();
