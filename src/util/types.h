#ifndef _TYPES_H_
#define _TYPES_H_

/*
/* User input
 * 
 * uas: user-agent string
 * class [optional]: use only this classifier
 * parsed_uas: a dictionary of uni-grams, bi-grams, and tri-grams of the 
 *             user-agent string together with their frequencies in the string
 * 
 */
struct input {
    char *uas;
    char *class;
    //struct parsed_uas *parsed_uas;
};

/*
 * uas_record *root points to the head of a linked list of user-agent strings.
 */
struct class_data {
    char *class;
    struct uas_record *head;
};

struct classifier {
    struct link_node_int *prior;
    struct link_node_float *p_prior;
    struct link_node_int *corpusDictionary;
    struct dict_link_node_int *classDictionary;
};


#endif