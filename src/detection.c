#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "ann.h"

// User-Agent string to work with
char *uas;

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
      add_argument(argv[i], argv[i+1]);
      i++; // expects the next argmuent be the value of the prev argument name
    }
  }

  // Interactive mode by default if no arguments passed
  if (argc == 1) {
    printf("Started without arguments, executing in an interactive mode\n");
    printf("Interactive mode not yet implemented. Exiting now!\n");
    return 1;
  }

  if(v) printf("Started with arguments, executing in cli mode\n");

  // Run it through the neural network
  if(v) printf("Working with user-agent string %s\n", uas);

  // Return results in JSON format to stdout

  return 0;
}

int add_argument(char *arg_name, char *arg_val)
{

  /*switch(*arg_name) {
  case "str" : 
    break;
  default:
    printf("Unknown argument %s", arg_name);
    }*/
  if(strcmp(arg_name, "--ua") == 0) { 
    uas = arg_val;
    if(v) printf("Adding: %s as %s\n", arg_name, arg_val);
  }else {
    printf("Unknown argument %s\n", arg_name);
  }
  return 0;
}
