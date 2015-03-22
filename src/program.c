#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#include "reader.h"
#include "htable_int.h"
#include "dictionary.h"
#include "bitmask.h"
#include "tokenizer.h"
#include "probab.h"

#undef HTABLE_MODULE
#undef HTABLE_TYPE
#define HTABLE_TYPE float
#include "htable_int.h"
#include "htable_int.c"

void chck_malloc(void *ptr, char *desc);
void read_data_with_class(char *path, struct uas_record *root, int *lc);
void save_data_bin();
void load_data_bin();
void train(struct uas_record *root, struct htable_int *prior);
void evaluate(struct htable_int *words, struct uas_record *uas_input);
void read_user_input(int argc, char **argv, struct uas_record *uas_input);
void print_usage();

/*
 * Corpus Dictionary keeps reference of each 'word' from the user agent
 * strings that have been read and keeps a counter for how many times
 * each word has been seen in the input.
 */
struct htable_int *corpusDict = NULL;
/*
 * Class Dictionary works similarly to Corpus Dictionary, it just keeps
 * the counts for each class separately.
 */
struct dict_htable_int *classDict = NULL;

/*
 * Prior keeps the count of how many times each label (class) has been
 * read in the input data
 */
struct htable_int *prior = NULL;

/*
 * P_prior - probability of each label (class) is computed as
 * P(class) = # class / # all classes
 */
struct htable_float *p_prior = NULL;

/*
 * Two auxiliary pointers that are used to iterate/dynamically create
 * new pointers to htable_int structs.
 */
struct htable_int *tmp = NULL;
struct htable_int *aux = NULL;

/*
 * Auxiliary pointers that are used to iterate/dynamically create new
 * pointers to dict_htable_int structs.
 */
struct dict_htable_int *thisClassDict = NULL;
struct dict_htable_int *tmpDict = NULL;
struct dict_htable_int *auxDict = NULL;

struct htable_int *iterator = NULL;
struct htable_float *p_iterator = NULL;

struct uas_record *root = NULL;

struct htable_int *words = NULL;

struct uas_record *uas_input = NULL;


float log_prob_word_class = 0;

/*
 * Bit mask setting given operations on/off
 */
unsigned short settings = 0;

const unsigned short LOAD_DATA_FILE_FLAG = 1;
const unsigned short LOAD_BIN_DATA_FLAG = 1 << 1;
const unsigned short TRAIN_ON_DATA_FLAG = 1 << 2;
const unsigned short DO_EVALUATE_FLAG = 1 << 3;

int main(int argc, char** argv) {

    /*
     * This program acts as an interface to the problems defined in the README.md:
     *
     *     1. What are the probabilities that the given user-agent string belongs
     *     to a pre-defined class (group)?
     *
     *     For this problem, you'd call this program with two parameters:
     *         ./program -ua <some user-agent string> -group <some group> [-treshold=0.8]
     *
     *     This will print "probably yes" or "probably no" given the P(ua|class) and
     *     the treshold.
     *
     *     It can also print the probabilities for each learned class, e.g.:
     *
     *     class1=0.909032
     *     class2=0.870211
     *     ...
     *
     *     Optionally, you can specify which format of the output you want:
     *     ./program ... -output application/json
     *
     */

    corpusDict = htable_int_create();
    chck_malloc((void *) corpusDict, "Corpus Level Dictionary");

    classDict = dict_htable_int_create();
    chck_malloc((void *) classDict, "Collection of Class Dictionaries");

    prior = htable_int_create();
    chck_malloc((void *) prior, "Array of Prior Classes");

    p_prior = htable_float_create();
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

    read_user_input(argc, argv, uas_input);

    /*
     * LEARNING PHASE
     * ==============
     *
     * I. Open data file with pairs (class, user-agent string)
     *
     *    set struct uas_record *current = NULL
     *    set struct uas_record *previous = NULL
     *
     * 	  For each line as current (class, user-agent string) do:
     *        set prior[class] += 1
     *
     *    This increments the counter that checks how many UA strings from
     *    each class we have encountered
     *
     *        count_words(current, corpusDict)
     *
     *    This will tokenize current->uas and add count for each unique word
     *    from that user-agent string to corpusDict
     *
     *        set struct htable_int thisClassDict = NULL
     *
     *        thisClassDict = dict_htable_int_find(classDict, class)
     *
     *    This will lookup the dictionary of words for the given class. If
     *    the class' dictionary does not exists yet, create it
     *
     *        if (thisClassDict == NULL)
     *            thisClassDict = htable_int_create()
     *
     *        count_words(current, thisClassDict)
     *
     *    Tokenize current->uas and add unique words count to the dictionary
     *    of the current class
     *
     *
     *
     * II. Evaluate category probabilities, i.e.
     *
     *     For each prior as (class, cnt):
     *
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
     *
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
     *     set struct htable_int words = NULL
     *     words = htable_int_create()
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
     *     set float treshold = 0.8
     *     set short belong_to_group = 0
     *     set char* group = argv // supplied group thru arguments
     *
     *     For each log_prob as (class, value) do:
     *
     *         print("%s=%f", class, value)
     *
     *         if (value >= treshold && strcmp(group, class) == 0):
     *             belong_to_group = 1
     *
     *     if (belong_to_group == 1):
     *         print("probably yes")
     *     else
     *         print("probably no")
     *
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

    if (mask_is_set_bool(&settings, &LOAD_DATA_FILE_FLAG))
        read_data_with_class("data/uas_with_class.txt", root, &lc);

    /*
     * TRAIN FROM THE DATA
     */
    if (mask_is_set_bool(&settings, &TRAIN_ON_DATA_FLAG))
        train(root, prior);

    /*
     * IV. READ USER INPUT
     */
    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG)) {
        /*
         * V. Compute the features of the user input
         */
        words = htable_int_create();
        chck_malloc((void *) words, "Words Table of the User Input");

        count_words(uas_input, words);

        /*
         * VI. Evaluate the user input
         */
        evaluate(words, uas_input);

        /*
         * Print the result:
         */
        float prior_class_val = 0;
        struct htable_float *aux_float = htable_float_get(p_prior, uas_input->class);

        // if found
        if (aux_float != NULL)
            prior_class_val = aux_float->val;

        printf("%s in %s = %f\n", uas_input->uas, uas_input->class, expf(log_prob_word_class + logf(prior_class_val)));
    }

    // Free up remaining resources:
    htable_int_free(corpusDict);
    htable_int_free(prior);
    htable_int_free(words);
    htable_float_free(p_prior);
    uas_record_free(root);
    uas_record_free(uas_input);
    dict_htable_int_free(classDict);

    return 0;
}

/*
 * malloc can return a NULL pointer if the allocation fails
 *
 * If *ptr is a pointer that has just been malloc'd and passed to this function
 * it will print an error and exit if *ptr is a NULL pointer.
 */
void chck_malloc(void *ptr, char *desc)
{
    if (ptr == NULL) {
        printf("Error malloc'ing for %s\n", desc);
        exit(1);
    }
}

void read_data_with_class(char *path, struct uas_record *root, int *lc)
{
    *lc = 0;

    *lc = read_uas_with_class(path, root);

    print_uas_records(root);
    printf("lines of data read = %d\n", *lc);
}

// root = root node of the uas_record linked list containing UA strings
void train(struct uas_record *root, struct htable_int *prior)
{
    struct uas_record *record = root;

    /*
     * (This assumes that there are at least 2 records, else the test
     * in the while loop will evaluate to false in the first pass already).
     */
    while (record->next) {
        /*
         * record the number of record->class that have been read:
         *
         * lookup the struct for the record->class and +1 its val
         * field. If not found, it's the first time this class has been
         * read so create a new struct and append it to the end link...
         */
        tmp = htable_int_get(prior, record->class);

        if (tmp) {
            tmp->val++;
        } else if (prior->name == NULL) {
            htable_int_set(prior, record->class, 1);
        } else {
            tmp = htable_int_get_last(prior);
            aux = htable_int_create();
            htable_int_set(aux, record->class, 1);
            tmp->next = aux;
        }

        /*
         * Increment the counters for each unique word # of occurrences
         */
        count_words(record, corpusDict);

        /*
         * Do the same for the class dictionary. If the class dictionary not
         * found, create a new one as we have a new class
         */
        thisClassDict = dict_htable_int_find(classDict, record->class);

        if (thisClassDict) {
            count_words(record, thisClassDict->root);
        } else if (classDict->root == NULL) {
            aux = htable_int_create();
            dict_htable_int_set(classDict, record->class, aux, NULL);
            count_words(record, classDict->root);
        } else {
            tmpDict = dict_htable_int_create();
            aux = htable_int_create();
            dict_htable_int_set(tmpDict, record->class, aux, NULL);
            count_words(record, tmpDict->root);

            auxDict = dict_htable_int_find_last(classDict);
            auxDict->next = tmpDict;
        }

        record = record->next;
    }

    iterator = prior;
    p_iterator = p_prior;

    int sum_prior_vals = htable_int_sum_val_rec(prior);
    float val;

    printf("sum_prior_vals = %d\n", sum_prior_vals);

    while(iterator) {
        printf("prior: class %s, cnt %d\n", iterator->name, iterator->val);
        val = (float)iterator->val / (float)sum_prior_vals;

        htable_float_set(p_iterator, iterator->name, val);
        p_iterator->next = htable_float_create();
        printf("P(%s) = %d / %d = %f\n", p_iterator->name, iterator->val, sum_prior_vals, p_iterator->val);

        p_iterator = p_iterator->next;
        iterator = iterator->next;
    }
}

/*
 * Config via command line arguments
 */
void read_user_input(int argc, char **argv, struct uas_record *uas_input)
{
    char *uas = NULL;
    char *class = NULL;

    int c;

    static struct option long_options[] = {
            { "group", required_argument, 0, 'a' },
            { "uas", required_argument, 0, 'b' },
            { "help", no_argument, 0, 'h' }
    };

    while (1) {
        int option_index = 0;

        c = getopt_long(argc, argv, "a:b:h", long_options, &option_index);

        if (c == -1)
            break;

        switch (c){
            case 0:
                break;
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
            default:
                printf("unrecognized option\n");
        }
    }

    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG) && class == NULL) {
        printf("WARNING: No class set! Using 'desktop' as default\n");
        class = malloc(sizeof(char) * 8);
        chck_malloc((void *) class, "Class string");
        strcpy(class, "default");
    }

    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG) && uas == NULL) {
        printf("wrong usage - must specify a user-agent string or disable the evaluation phase\n");
        print_usage();
        uas_record_free(root);
        htable_int_free(corpusDict);
        dict_htable_int_free(classDict);
        htable_int_free(prior);
        htable_float_free(p_prior);
        exit(1);
    }

    printf("using input = %s\n", uas);
    uas_record_set(uas_input, class, uas, NULL);

    if (class != NULL) free(class);
    if (uas != NULL) free(uas);
}

/*
 * Evaluates user input:
 *
 *     *words: tokenized keywords of the user-agent string
 *     *uas_input: the user-agent string and the class
 */
void evaluate(struct htable_int *words, struct uas_record *uas_input)
{
    float p_word = 0;
    float p_word_class = 0;

    int corpusDict_word_val = 0;

    iterator = words;

    /*
     * Loop through each words of the input user-agent string
     */
    while(iterator) {
        /*
         * Look up the frequency of the word in the corpusDict.
         */
        aux = htable_int_get(corpusDict, iterator->name);

        /*
         * If the word is not in the dictionary there is no data on its prior
         * probability so the program skips the loop
         */
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }

        /*
         * Calculate the probability that the word appears at all (in all
         * known user-agent strings).
         */
        p_word = (float)aux->val / (float)htable_int_sum_val_rec(corpusDict);

        printf("P(%s) = %d / %d = %f\n", iterator->name, aux->val, htable_int_sum_val_rec(corpusDict), p_word);

        /*
         * Now look up the dictionary of the given class
         */
        thisClassDict = dict_htable_int_find(classDict, uas_input->class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", uas_input->class);
            iterator = iterator->next;
            continue;
        }

        /*
         * Look up the frequency of the word in the class' dictionary
         */
        aux = htable_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
        /*
         * Calculate the probability that the word appears in the given
         * class.
         */
        p_word_class = (float)aux->val / (float)htable_int_sum_val_rec(thisClassDict->root);

        printf("P(%s|%s) = %d / %d = %f\n", iterator->name, uas_input->class, aux->val, htable_int_sum_val_rec(thisClassDict->root), p_word_class);

        //
        //if (P(word|class) > 0:
        //     *             log_prob[class] += log(count * P(word|class) / P(word))
        if (p_word_class > 0) {
            log_prob_word_class += logf((float)iterator->val * p_word_class / p_word);
        }

        iterator = iterator->next;
    }
}

void print_usage()
{
    printf("\nua-detection usage:\n\n");
    printf("\t--uas <user-agent string> to specify user-agent string\n");
    printf("\t--group <group> to specify the group classifier\n");
    printf("\t--help prints this help\n");
    printf("\n");
}
