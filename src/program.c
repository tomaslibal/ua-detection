/**
 * This file includes the main() entry point for `uadet` program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#include "reader.h"
#include "dictionary.h"
#include "bitmask.h"
#include "link_node.h"
#include "tokenizer.h"
#include "probab.h"

/*
 * Undefine LINK_NODE_H so that it can define "link_node_float" as the header
 * file link_node.h by default defines "link_node_int".
 *
 * The difference is that link_node_float is a struct that holds float values
 * in node->(float)val
 */
#undef LINK_NODE_H
#undef NODE_TYPE
#define NODE_TYPE float
#include "link_node.h"
#include "link_node.c"


void chck_malloc(void *ptr, char *desc);
void read_data_with_class(char *path, struct uas_record *root, int *lc);
void read_cls_data(char *class_name, struct uas_record *root, int *lc);
void save_data_bin();
void load_data_bin();
void train(struct uas_record *root, struct link_node_int *prior);
void evaluate(struct link_node_int *words, struct uas_record *uas_input);
float evaluate_cls(struct link_node_int *words, struct uas_record *uas_input, char *class);
void cmp_all(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input);
void cmp_one(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input, const char *class);
void read_CLI_input(int argc, char **argv, struct uas_record *uas_input);
void print_usage();
void free_shared_res();

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
        train(root, prior);

    /*
     * IV. READ USER INPUT
     */
    if (mask_is_set_bool(&settings, &DO_EVALUATE_FLAG)) {
        /*
         * V. Compute the features of the user input
         */
        words = link_node_int_create();
        chck_malloc((void *) words, "Words Table of the User Input");

        count_words(uas_input, words);

        //
        float prior_class_val = 0;
        float log_prob = 0;
        float a = 0.0;
        float b = 0.0;
        struct link_node_float *aux_float = NULL;

        /*
         * CMP_ALL_CLS_FLAG
         * Compare the user-agent against all classifiers
         */
        if (mask_is_set_bool(&settings, &CMP_ALL_CLS_FLAG)) {
            cmp_all(prior, words, uas_input);
        /*
         * Or compare against the one given class
         */
        } else {
            cmp_one(prior, words, uas_input, uas_input->class);
        }
    }

    free_shared_res();

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

    //print_uas_records(root);
    printf("lines of data read = %d\n", *lc);
}

/*
 * one-vs-all strategy: read a separate <class_name>.cls.txt data file which contains
 * pairs <class, uas> where class = { <class_name>, other }
 */
void read_cls_data(char *class_name, struct uas_record *root, int *lc)
{
    char *path = NULL;
    *lc = 0;

    path = malloc(sizeof(char) * strlen(class_name) + sizeof(char) * 13);
    sprintf(path, "data/%s.cls.txt", class_name);
    *lc = read_uas_with_class(path, root);
    printf("read %d lines of data\n", *lc);

    free(path);
}

// root = root node of the uas_record linked list containing UA strings
void train(struct uas_record *root, struct link_node_int *prior)
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
        tmp = link_node_int_get(prior, record->class);

        if (tmp) {
            tmp->val++;
        } else if (prior->name == NULL) {
            link_node_int_set(prior, record->class, 1);
        } else {
            tmp = link_node_int_get_last(prior);
            aux = link_node_int_create();
            link_node_int_set(aux, record->class, 1);
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
        thisClassDict = dict_link_node_int_find(classDict, record->class);

        if (thisClassDict) {
            count_words(record, thisClassDict->root);
        } else if (classDict->root == NULL) {
            aux = link_node_int_create();
            dict_link_node_int_set(classDict, record->class, aux, NULL);
            count_words(record, classDict->root);
        } else {
            tmpDict = dict_link_node_int_create();
            aux = link_node_int_create();
            dict_link_node_int_set(tmpDict, record->class, aux, NULL);
            count_words(record, tmpDict->root);

            auxDict = dict_link_node_int_find_last(classDict);
            auxDict->next = tmpDict;
        }

        record = record->next;
    }

    iterator = prior;
    p_iterator = p_prior;

    int sum_prior_vals = link_node_int_sum_val_rec(prior);
    float val;

    //printf("sum_prior_vals = %d\n", sum_prior_vals);

    while(iterator) {
        //printf("prior: class %s, cnt %d\n", iterator->name, iterator->val);
        val = (float)iterator->val / (float)sum_prior_vals;

        link_node_float_set(p_iterator, iterator->name, val);
        p_iterator->next = link_node_float_create();
        //printf("P(%s) = %d / %d = %f\n", p_iterator->name, iterator->val, sum_prior_vals, p_iterator->val);

        p_iterator = p_iterator->next;
        iterator = iterator->next;
    }
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

float evaluate_cls(struct link_node_int *words, struct uas_record *uas_input, char *class)
{
    float p_word = 0;
    float p_word_class = 0;

    float log_prob = 0;

    int corpusDict_word_val = 0;

    iterator = words;

    /*
     * Loop through each words of the input user-agent string
     */
    while(iterator) {
        /*
         * Look up the frequency of the word in the corpusDict.
         */
        aux = link_node_int_get(corpusDict, iterator->name);

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
        p_word = (float)aux->val / (float)link_node_int_sum_val_rec(corpusDict);

        //printf("P(%s) = %d / %d = %f\n", iterator->name, aux->val, link_node_int_sum_val_rec(corpusDict), p_word);

        /*
         * Now look up the dictionary of the given class
         */
        thisClassDict = dict_link_node_int_find(classDict, class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", class);
            iterator = iterator->next;
            continue;
        }

        /*
         * Look up the frequency of the word in the class' dictionary
         */
        aux = link_node_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
        /*
         * Calculate the probability that the word appears in the given
         * class.
         */
        p_word_class = (float)aux->val / (float)link_node_int_sum_val_rec(thisClassDict->root);

        //printf("P(%s|%s) = %d / %d = %f\n", iterator->name, class, aux->val, link_node_int_sum_val_rec(thisClassDict->root), p_word_class);

        //
        //if (P(word|class) > 0:
        //     *             log_prob[class] += log(count * P(word|class) / P(word))
        if (p_word_class > 0) {
            log_prob += logf((float)iterator->val * p_word_class / p_word);
        }

        iterator = iterator->next;
    }

    return log_prob;
}

/*
 * Evaluates user input:
 *
 *     *words: tokenized keywords of the user-agent string
 *     *uas_input: the user-agent string and the class
 */
void evaluate(struct link_node_int *words, struct uas_record *uas_input)
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
        aux = link_node_int_get(corpusDict, iterator->name);

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
        p_word = (float)aux->val / (float)link_node_int_sum_val_rec(corpusDict);

        //printf("P(%s) = %d / %d = %f\n", iterator->name, aux->val, link_node_int_sum_val_rec(corpusDict), p_word);

        /*
         * Now look up the dictionary of the given class
         */
        thisClassDict = dict_link_node_int_find(classDict, uas_input->class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", uas_input->class);
            iterator = iterator->next;
            continue;
        }

        /*
         * Look up the frequency of the word in the class' dictionary
         */
        aux = link_node_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
        /*
         * Calculate the probability that the word appears in the given
         * class.
         */
        p_word_class = (float)aux->val / (float)link_node_int_sum_val_rec(thisClassDict->root);

        //printf("P(%s|%s) = %d / %d = %f\n", iterator->name, uas_input->class, aux->val, link_node_int_sum_val_rec(thisClassDict->root), p_word_class);

        //
        //if (P(word|class) > 0:
        //     *             log_prob[class] += log(count * P(word|class) / P(word))
        if (p_word_class > 0) {
            log_prob_word_class += logf((float)iterator->val * p_word_class / p_word);
        }

        iterator = iterator->next;
    }
}

void cmp_all(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input)
{
    // each class must have a *.cls.txt file in the data folder
    // so list that folder and evaluate uas_input for each class
    // found in that directory

    // for now, manually override to mobile,desktop,android
    struct link_node_int *tmp = NULL;
    struct link_node_int *cls_iterator = NULL;
    float prior_class_val = 0;
    float log_prob = 0;
    float a = 0.0;
    float b = 0.0;
    struct link_node_float *aux_float = NULL;
    int lc = 0;

    cls_iterator = link_node_int_create();
    link_node_int_set(cls_iterator, "mobile", 0);
    tmp = link_node_int_create();
    link_node_int_set(tmp, "desktop", 0);
    cls_iterator->next = tmp;
    tmp = link_node_int_create();
    link_node_int_set(tmp, "android", 0);
    cls_iterator->next->next = tmp;

    while(cls_iterator) {
        read_cls_data(cls_iterator->name, root, &lc);
        train(root, prior);

        prior_class_val = 0;
        aux_float = link_node_float_get(p_prior, cls_iterator->name);
        if (aux_float != NULL)
            prior_class_val = aux_float->val;

        log_prob = evaluate_cls(words, uas_input, cls_iterator->name);

        a = expf(log_prob + logf(prior_class_val));

        prior_class_val = 0;
        aux_float = link_node_float_get(p_prior, "other");
        if (aux_float != NULL)
            prior_class_val = aux_float->val;

        log_prob = evaluate_cls(words, uas_input, "other");

        b = expf(log_prob + logf(prior_class_val));

        //printf("[cls:output %s] = %f\n", cls_iterator->name, a);
        //printf("[cls:output non-%s] = %f\n", cls_iterator->name, b);
        printf("[cls:%s]=%f%%\n", cls_iterator->name, (a / (a+b))*100);
        cls_iterator = cls_iterator->next;
    }
}

void cmp_one(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input, const char *class)
{
    float prior_class_val = 0;
    float log_prob = 0;
    float a = 0.0;
    float b = 0.0;
    struct link_node_float *aux_float = NULL;

    /*
     * VI. Evaluate the user input
     */
    evaluate(words, uas_input);

    /*
     * Print the result:
     */

    aux_float = link_node_float_get(p_prior, uas_input->class);

    // if found
    if (aux_float != NULL)
        prior_class_val = aux_float->val;

    a = expf(log_prob_word_class + logf(prior_class_val));
    //printf("[cls:output %s] = %f\n", uas_input->class, a);

    // -vs-all:

    prior_class_val = 0;
    aux_float = link_node_float_get(p_prior, "other");
    if (aux_float != NULL)
        prior_class_val = aux_float->val;

    log_prob = evaluate_cls(words, uas_input, "other");

    b = expf(log_prob + logf(prior_class_val));
    //printf("[cls:output non-%s] = %f\n", uas_input->class, b);

    printf("[cls:%s]=%f%%\n", uas_input->class, (a / (a+b))*100);
}

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
    if (root != NULL)       uas_record_free(root);
    if (uas_input != NULL)  uas_record_free(uas_input);
    if (prior != NULL)      link_node_int_free(prior);
    if (corpusDict != NULL) link_node_int_free(corpusDict);
    if (p_prior != NULL)    link_node_float_free(p_prior);
    if (classDict != NULL)  dict_link_node_int_free(classDict);
    if(words != NULL) link_node_int_free(words);
}
