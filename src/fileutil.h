/*
 * fileutil.h
 *
 *  Created on: Mar 23, 2015
 *      Author: tmlbl
 */

#ifndef SRC_FILEUTIL_H_
#define SRC_FILEUTIL_H_

#include <stdlib.h>

#include "reader.h"

// Higher level

/*
 * Saves a linked list of uas_record to a binary file.
 *
 * Uses save_struct();
 */
void save_read_uas_record();

/*
 * Loads a linked list of uas_record from a binary file to the memory
 */
void load_read_uas_record(char *path, struct uas_record *root);

// Low level

/*
 * Generic function that saves a linked list of structs into a binary file
 *
 * *path to the file
 * size in bits of each struct
 * *mode e.g. w or w+
 *
 * Returns 0 if successfully saved
 */
int save_struct(char *path, size_t struct_size, char *mode);


#endif /* SRC_FILEUTIL_H_ */
