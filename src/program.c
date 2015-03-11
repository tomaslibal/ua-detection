#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "htable_int.h"
#include "tokenizer.h"
#include "probab.h"

int main(int argc, char** argv) {

    /*
     This program acts as an interface to the problems defined in the README.md:

     1. Does a user-agent string belong to a pre-defined group?

     2. What device could the user-agent be sent from?

     For the first problem, you'd call this program with two parameters:
     ./program -ua <some user-agent string> -group <some group>

     For the second problem, you'd call this program with one parameter only:
     ./program -ua <some user-agent string>

     Optionally, you can specify which format of the output you want:
     ./program ... -output application/json

    */

	/*
	 * Provisionally, this program does the following:
	 *
	 * NB lines starting with ';;' are 'commented out'
	 *
	 * 1. Open data/uas_with_class.txt
	 *
	 * 2. Read records from the txt file line by line
	 *
	 * 3. Store each line as a uas_record struct, pointing to the next one
	 *    in a linked list structure
	 *        uas_record(*p, class_name, uas, next)
	 *
	 * 4. Let corpusDict = new htable_int struct
	 *
	 *    Let classDict = new classesDictionaries
	 * ;; array of arrays not yet implemented:
	 * ;; (classDict['mobile'][...vocab...]
	 * ;;  classDict['desktop'][...vocab...]
	 * ;;  ...)
	 *
	 * 5.
	 *    5a. For each uas_record, tokenize the user agent string
	 *
	 *    5b. For each word in the user agent string
	 *     	    5b I. add word to the corpusDict
	 * ;;           ;; 5b II. add word to the classDict of the current class
	 *
	 * 6. free the linked list of user-agent strings
	 * 	  free the linked list corpusDict
	 *
	 * 7. close the file
	 */

	FILE *fp = NULL;

	fp = fopen("../data/uas_with_class.txt", "r");

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
	while(record->next) {
		/*
		 * 5a I.
		 */
		count_words(record, corpusDict);
		record = record->next;
	}

	struct htable_int *iterator = NULL;

	iterator = corpusDict;

	while(iterator->next) {
		printf("corpusDict[%s] %dx\n", iterator->name, iterator->val);
		iterator = iterator->next;
	}

	uas_record_free(root);
	htable_int_free(corpusDict);

	fclose(fp);

    return 0;
}
