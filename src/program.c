#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "htable_int.h"
#include "tokenizer.h"
#include "probab.h"

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
     * 0. Some administrivia
     *
     *    set struct htable_int corpusDict = NULL
     *    set struct dict_htable_int classDict = NULL
     *
     *    set struct htable_int p_prior = NULL
     *
     *    corpusDict = htable_int_create();
     *    classDict = dict_htable_int_create();
     *
     *    p_prior = htable_int_create();
     *
     *
     * I. Open data file with pairs (class, user-agent string)
     *
     *    set struct uas_record current = NULL
     *    set struct uas_record previous = NULL
     *
     * 	  For each line = (class, user-agent string) do:
     *        set prior[class] += 1
     *
     *    This increments the counter that checks how many UA strings from
     *    each class we have encountered
     *
     *        if (current == NULL)
     *            current = uas_record_create()
     *
     *        uas_record_set(current, class, user-agent string, NULL)
     *
     *        if (previous != NULL)
     *            previous->next = current
     *        else
     *            previous = current
     *
     *    This will create a new uas_record from each line of data file
     *    and link them together in the order in which they have been read.
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
     *         p_prior[class]->val = prior[class]->val / sum(all other priors->val)
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
     * V. Analyze the input
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
     *             log_prob[class] += log(count * P(word|category) / P(word))
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
     *     htable_int_free(corpusDict)
     *     dict_htable_int_free(classDict)
     *     fclose(fp) if using file as an input
     *     htable_int_free(p_prior)
     *     htable_int_free(log_prob)
     */

    struct uas_record *root = NULL;
    struct uas_record *record = NULL;

    root = uas_record_create();

    if (root == NULL) {
        exit(1);
    }

    /*
     * line count of the txt file read
     */
    int lc = 0;

    lc = read_uas_with_class("test/resources/uas_with_class.txt", root);

    print_uas_records(root);

    /*
     * 4.
     */
    struct htable_int *corpusDict = NULL;

    corpusDict = htable_int_create();

    record = root;

    /*
     * 5.
     */
    while (record->next) {
        /*
         * 5a I.
         */
        count_words(record, corpusDict);
        record = record->next;
    }

    struct htable_int *iterator = NULL;

    iterator = corpusDict;

    while (iterator->next) {
        printf("corpusDict[%s] %dx\n", iterator->name, iterator->val);
        iterator = iterator->next;
    }

    uas_record_free(root);
    htable_int_free(corpusDict);

    return 0;
}
