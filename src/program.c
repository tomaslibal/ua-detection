/**
 * This file includes the main() entry point for `uadet` program.
 * 
 * Uadet is a software that uses a probabilistic approach to machine learning
 * and its domain is classification of user-agent strings.
 *
 *  May 2015
 * 
 *  License: GNU 2.0 Free Software. See the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "util/bitmask.h"
#include "util/logging.h"
#include "util/types.h"
#include "util/memutil.h"
#include "model/bayes.h"

/*
 * Corpus Dictionary keeps a reference to each 'word' from the user agent
 * strings that have been read and keeps a counter for each word to track
 * how many times the word has been seen in the input.
 */
struct link_node_int *corpusDict = NULL;

/*
 * Class Dictionary works similarly to Corpus Dictionary, it just keeps
 * the counts for each class separately.
 */
struct dict_link_node_int *classDict = NULL;

/*
 * Prior keeps the count of how many times each label (class) has been
 * read in the input data
 */
struct link_node_int *prior = NULL;

/*
 * P_prior - probability of each label (class) is computed as
 * P(class) = # class / # all classes
 */
struct link_node_float *p_prior = NULL;

/*
 * Two auxiliary pointers that are used to iterate/dynamically create
 * new pointers to link_node_int structs.
 */
struct link_node_int *tmp = NULL;
struct link_node_int *aux = NULL;

/*
 * Auxiliary pointers that are used to iterate/dynamically create new
 * pointers to dict_link_node_int structs.
 */
struct dict_link_node_int *thisClassDict = NULL;
struct dict_link_node_int *tmpDict = NULL;
struct dict_link_node_int *auxDict = NULL;

/*
 * Auxiliary pointers to iterate over link_node_int and link_node_float structs
 */
struct link_node_int *iterator = NULL;
struct link_node_float *p_iterator = NULL;

/*
 * As each line of the data file is read (which is a touple of <class, user
 * agent string>) it is stored in this linked list of structs.
 */
struct uas_record *root = NULL;

/*
 * *words is a linked list of <word, count> touples where word is each token
 * of a user-agent string and count is the number of how many times the given
 * word appears in the string.
 */
struct link_node_int *words = NULL;

/*
 * The touple <class, user-agent string> of the user input is stored in this
 * struct.
 *
 * The class in the user input is not required if the user does not wish to
 * get a prediction for a given class but rather for all classes.
 */
struct uas_record *uas_input = NULL;
/*
 * Keeps the score (result of the classification) for the user input
 */
float log_prob_word_class = 0;

/*
 * Uses getopt_long to set bits in a shared "int settings" based on user's
 * command line input. 
 */
void read_CLI_input(int argc, char **argv, struct uas_record *uas_input);

/*
 * This prints a manually typed usage info. If API changes this function 
 * won't autoupdate. 
 */
void print_usage();

/*
 * Frees the program-wide shared resources if they are not a null pointer
 */
void free_shared_res();


/*
 * Bit mask setting given operations on/off
 */
unsigned short settings = 0;

/*
 * If this flag set, the program will open data/uas_with_class.txt and load the
 * content of this file for further processing
 */
const unsigned short LOAD_DATA_FILE_FLAG = 1;

/*
 * This flag has currently no effect. It is reserved for later use when it saving
 * the output of the training phase into a binary file is implemented. In that
 * case, when this flag set, it will load that binary file
 */
const unsigned short LOAD_BIN_DATA_FLAG = 1 << 1;

/*
 * If this flag set and LOAD_DATA_FILE_FLAG is also set, the program will learn
 * from the data loaded from the text data file data/uas_with_class.txt.
 */
const unsigned short TRAIN_ON_DATA_FLAG = 1 << 2;

/*
 * If this flag set, the program expects CLI input (at least a given user-agent
 * string) and the program will try to predict which class the input belongs to
 */
const unsigned short DO_EVALUATE_FLAG = 1 << 3;

/*
 * If this flag set and the DO_EVALUATE_FLAG is also set, the program will
 * compare the input against all known classes.
 */
const unsigned short CMP_ALL_CLS_FLAG = 1 << 4;

int main(int argc, char** argv) {

    /*
     * This program acts as an interface to the problems defined in the README.md:
     *
     *     1. What are the probabilities that the given user-agent string belongs
     *     to a pre-defined class (group)?
     *
     *     For this problem, you'd call this program with two parameters:
     *         ./program --uas <some user-agent string> --group <some group> [-treshold=0.8]
     *
     *     This will print P(uas|class)
     *
     *     It can also print the probabilities for each learned class, e.g.:
     *
     *         ./program --uas <uas> --cmp_all
     *
     *     This will print P(uas|class) for each class for which it has training
     *     data (in data/{class}.cls.txt)
     *
     *     NOT YET IMPLEMENTED:
     *     Optionally, you can specify which format of the output you want:
     *     ./program ... --output json
     *
     */

    corpusDict = link_node_int_create();
    chck_malloc((void *) corpusDict, "Corpus Level Dictionary");

    classDict = dict_link_node_int_create();
    chck_malloc((void *) classDict, "Collection of Class Dictionaries");

    prior = link_node_int_create();
    chck_malloc((void *) prior, "Array of Prior Classes");

    p_prior = link_node_float_create();
    chck_malloc((void *) p_prior, "Array of P(class) probabilities");

    /*
     * Apply default settings
     *
     */
    mask_set(&settings, &LOAD_DATA_FILE_FLAG);
    mask_set(&settings, &TRAIN_ON_DATA_FLAG);
    mask_set(&settings, &DO_EVALUATE_FLAG);

    /*
     * Read user input - configuration
     */
    uas_input = uas_record_create();
    chck_malloc((void *) uas_input, "UAS Struct for the User Input of Data");

    read_CLI_input(argc, argv, uas_input);

    /*
     * LEARNING PHASE
     * ==============
     *
     * I. Open data file with pairs (class, user-agent string)
     *
     * 	  For each line as current = (class, user-agent string) do:
     *        set prior[class] += 1
     *
     *    This increments the counter that checks how many UA strings from
     *    each class we have encountered.
     *
     *    Then do:
     *        count_words(current, corpusDict)
     *
     *    This will tokenize current->uas and add count for each unique word
     *    from that user-agent string to corpusDict.
     *
     *    Then do:
     *        set struct link_node_int thisClassDict = NULL
     *
     *        thisClassDict = dict_link_node_int_find(classDict, class)
     *
     *    This will lookup the dictionary of words for the given class. If
     *    the class' dictionary does not exists yet, create it:
     *
     *        if (thisClassDict == NULL)
     *            thisClassDict = link_node_int_create()
     *
     *    Then do:
     *        count_words(current, thisClassDict)
     *
     *    Tokenize current->uas and add unique words count to the dictionary
     *    of the current class.
     *
     *
     *
     * II. Evaluate categories' probabilities, i.e.
     *
     *     For each prior as (class, cnt) do:
     *         p_prior[class]->val = prior[class]->val / sum(all priors->val)
     *
     *     This will evaluate P(class) probabilities.
     *
     *
     *
     * EVALUATE NEW SAMPLE PHASE
     * =========================
     *
     * III. Initialize log_prob which will hold the probability for each class
     *
     *     For each prior as (class, ...) do:
     *         log_prob[class] = 0.0
     *
     *
     * IV. Read the UA from the arguments/file
     *
     *     set char *uas_input = malloc((sizeof(char) * argv) + 1)
     *     strcpy(uas_input, argv)
     *
     *
     * V. Process the input
     *
     *     set struct link_node_int words = NULL
     *     words = link_node_int_create()
     *
     *     count_words(uas_input, words)
     *
     *
     * VI. For each words as (word, count) do:
     *
     *         P(word) = corpusDict[word] / sum(corpusDict all words #)
     *
     *         P(word|class) = classDict[class][word] / sum(classDict[class] all words in class #)
     *
     *         if (P(word|class) > 0:
     *             log_prob[class] += log(count * P(word|class) / P(word))
     *
     * VII. Print results
     *
     *     ....
     *
     * VIII. CLEAN UP
     *
     *     Free up the resources
     */


    /*
     * READ  THE UAS DATA
     */
    int lc = 0;

    root = uas_record_create();
    chck_malloc((void *) root, "Array of UAS Records");

    /*
     * When CMP_ALL is set, it is most likely that the user has supplied only
     * a user-agent string (and no class). Function read_cls_data would not
     * know which class's training set to load, so here we disable the file
     * loading altogether, as it happens later on in function cmp_all which
     * will load a training set for each class separately.
     */
    if(mask_is_set_bool(&settings, &CMP_ALL_CLS_FLAG))
        mask_unset(&settings, &LOAD_DATA_FILE_FLAG);

    if (mask_is_set_bool(&settings, &LOAD_DATA_FILE_FLAG))
        read_cls_data(uas_input->class, root, &lc);

    /*
     * TRAIN FROM THE DATA
     */
    if (mask_is_set_bool(&settings, &TRAIN_ON_DATA_FLAG))
        train(root, prior, corpusDict, classDict, p_prior);

    /*
     * Evaluation Phase
     *
     * 1) Evaluate a given UA-String against all available classifiers
     * 2) Evaluate a given UA-String against one classifier
     */
    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG)) {

        /*
         * V. Compute the features of the user input
         */
        words = link_node_int_create();
        chck_malloc((void *) words, "Words Table of the User Input");

        count_words(uas_input, words);

        /*
         * CMP_ALL_CLS_FLAG set to true means that we're to compare
         * the user-agent against all classifiers
         */
        if (mask_is_set_bool(&settings, &CMP_ALL_CLS_FLAG)) {
            cmp_all(prior, words, uas_input, p_prior);
        /*
         * Else compare against the one given class (which is stored in
         * uas_input->class).
         */
        } else {
            cmp_one(words, uas_input, corpusDict, classDict, p_prior);
        }
    }

    free_shared_res();
    
    //save_data_bin("test", p_prior, corpusDict, classDict);

    return 0;
}

/*
 * Config via command line arg  uments
 */
void read_CLI_input(int argc, char **argv, struct uas_record *uas_input)
{
    char *uas = NULL;
    char *class = NULL;

    int c;

    static struct option long_options[] = {
            { "group", required_argument, 0, 'a' },
            { "uas", required_argument, 0, 'b' },
            { "help", no_argument, 0, 'h' },
            { "cmp_all", no_argument, 0, 'c' }
    };

    while (1) {
        int option_index = 0;

        c = getopt_long(argc, argv, "a:b:h", long_options, &option_index);

        if (c == -1)
            break;

        switch (c){
            case 'h':
                print_usage();
                exit(0);
                break;
            case 'a':
                class = malloc(sizeof(char) * strlen(optarg) + 1);
                chck_malloc((void *) class, "Class String");
                strcpy(class, optarg);
                break;
            case 'b':
                uas = malloc(sizeof(char) * strlen(optarg) + 1);
                chck_malloc((void *) uas, "User-agent String");
                strcpy(uas, optarg);
                break;
            case 'c':
                mask_set(&settings, &CMP_ALL_CLS_FLAG);
                break;
        }
    }

    /*
     * If the DO_EVALUATE_FLAG is set the program will attempt to classify
     * the given user-agent string and therefore it might need to know which
     * classifier the user wants to use.
     *
     * There is another flag CMP_ALL_CLS_FLAG which, if set, will evalute
     * the user-agent string againts all classifiers. In this case we do not
     * need to fallback to a default class.
     */
    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG) && class == NULL) {
        printf("WARNING: No class set! Using 'desktop' as default\n");
        class = malloc(sizeof(char) * 8);
        chck_malloc((void *) class, "Class string");
        strcpy(class, "default");
    }

    /*
     * If the DO_EVALUATE_FLAG is the program will attemp to classify the given
     * user-agent string and therefore the user-agent input is required.
     */
    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG) && uas == NULL) {
        printf("wrong usage - must specify a user-agent string or disable the evaluation phase\n");
        print_usage();

        free_shared_res();
        if (class != NULL) free(class);
        if (uas != NULL) free(uas);
        exit(1);
    }

    printf("using input = %s\n", uas);
    uas_record_set(uas_input, class, uas, NULL);

    if (class != NULL) free(class);
    if (uas != NULL) free(uas);
}


/*
 * Prints how to use the program from the command line interface
 */
void print_usage()
{
    printf("\nua-detection usage:\n\n");
    printf("\t--uas <user-agent string> to specify user-agent string\n");
    printf("\t--group <group> to specify the group classifier\n");
    printf("\t--cmp_all to compare the user-agent against all classifiers\n");
    printf("\t--help prints this help\n");
    printf("\n");
}

void free_shared_res()
{
    if (root       != NULL)  uas_record_free(root);
    if (uas_input  != NULL)  uas_record_free(uas_input);
    if (prior      != NULL)  link_node_int_free(prior);
    if (corpusDict != NULL)  link_node_int_free(corpusDict);
    if (p_prior    != NULL)  link_node_float_free(p_prior);
    if (classDict  != NULL)  dict_link_node_int_free(classDict);
    if (words      != NULL)  link_node_int_free(words);
}