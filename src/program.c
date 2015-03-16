#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "reader.h"
#include "htable_int.h"
#include "dictionary.h"

#include "tokenizer.h"
#include "probab.h"

#undef HTABLE_MODULE
#undef HTABLE_TYPE
#define HTABLE_TYPE float
#include "htable_int.h"
#include "htable_int.c"

int main(int argc, char** argv) {

    /*
     This program acts as an interface to the problems defined in the README.md:

     1. What are the probabilities that the given user-agent string belongs
     to a pre-defined class (group)?

     For this problem, you'd call this program with two parameters:
     ./program -ua <some user-agent string> -group <some group> [-treshold=0.8]

     This will print "probably yes" or "probably no" given the P(ua|class) and
     the treshold.

     It can also print the probabilities for each learned class, e.g.:

         class1=0.909032
         class2=0.870211
         ...

     Optionally, you can specify which format of the output you want:
     ./program ... -output application/json

     */

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

    struct htable_int *tmp = NULL;
    struct htable_int *aux = NULL;

    struct dict_htable_int *thisClassDict = NULL;
    struct dict_htable_int *tmpDict = NULL;
    struct dict_htable_int *auxDict = NULL;

    corpusDict = htable_int_create();

    if (corpusDict == NULL) {
        printf("Unable to create the corpus level dictionary!\n");
        exit(1);
    }

    classDict = dict_htable_int_create();

    if (classDict == NULL) {
        printf("Unable to create the class dictionary!\n");
        exit(1);
    }

    prior = htable_int_create();

    if (prior == NULL) {
        printf("Unable to create an array of prior classes\n");
        exit(1);
    }

    p_prior = htable_float_create();

    if (p_prior == NULL) {
        printf("Unable to create an array for P(class) probabilities\n");
        exit(1);
    }

    /*
     * READ UAS DATA
     */

    struct uas_record *root = NULL;
    struct uas_record *record = NULL;

    root = uas_record_create();

    if (root == NULL) {
        printf("Unable to create the UAS records array\n");
        exit(1);
    }

    /*
     * line count of the txt file read
     */
    int lc = 0;

    lc = read_uas_with_class("data/uas_with_class.txt", root);

    print_uas_records(root);

    record = root;

    printf("lines of data read = %d\n", lc);

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

    /*
     * p_prior[class] probabilities from prior[class]
     */
    struct htable_int *iterator = NULL;
    struct htable_float *p_iterator = p_prior;

    iterator = prior;

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
    //

    char *uas = NULL;
    char *class = NULL;
    struct uas_record *uas_input = NULL;

    // if argc == 5, assume argv[2] is the user agent string and argv[4] the class
    if (argc == 5) {
        uas = malloc(sizeof(char) * strlen(argv[2]) + 1);
        strcpy(uas, argv[2]);
        class = malloc(sizeof(char) * strlen(argv[4]) + 1);
        strcpy(class, argv[4]);
    } else {
        printf("wrong usage\n");
        exit(1);
    }

    printf("using input = %s\n", uas);

    // V.
    struct htable_int *words = NULL;
    words = htable_int_create();

    uas_input = uas_record_create();
    if (uas_input == NULL) {
        printf("Error creating uas_record for the input\n");
        exit(1);
    }

    uas_record_set(uas_input, class, uas, NULL);

    free(class);

    count_words(uas_input, words);

    //
    iterator = corpusDict;

    while (iterator) {
        printf("corpusDict[%s] %dx\n", iterator->name, iterator->val);
        iterator = iterator->next;
    }

    // VI.
    float p_word = 0;
    float p_word_class = 0;

    float log_prob_word_class = 0;

    int corpusDict_word_val = 0;

    iterator = words;

    while(iterator) {
        aux = htable_int_get(corpusDict, iterator->name);
        // word not in the dictionary
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }

        p_word = (float)aux->val / (float)htable_int_sum_val_rec(corpusDict);

        printf("P(%s) = %d / %d = %f\n", iterator->name, aux->val, htable_int_sum_val_rec(corpusDict), p_word);

        thisClassDict = dict_htable_int_find(classDict, uas_input->class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", uas_input->class);
            iterator = iterator->next;
            continue;
        }

        // word must be in the class dict as it was in the corpus dict
        aux = htable_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
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

    // result
    float prior_class_val = 0;
    struct htable_float *aux_float = htable_float_get(p_prior, uas_input->class);
    prior_class_val = aux_float->val;
    printf("%s in %s = %f\n", uas_input->uas, uas_input->class, expf(log_prob_word_class + logf(prior_class_val)));

    uas_record_free(root);
    htable_int_free(corpusDict);
    dict_htable_int_free(classDict);
    htable_int_free(prior);
    htable_float_free(p_prior);

    htable_int_free(words);
    if (uas != NULL)
        free(uas);
    uas_record_free(uas_input);

    return 0;
}
