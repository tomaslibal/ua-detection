#ifndef READER
#define READER

struct uas_record {
	char *uas;
	char *class;
	struct uas_record *next;
};

/*
 * Reads file specified by the 'path' argument. Assumes the file has one entry
 * per row in this format:
 *
 *     <class_name> <user_agent_string>
 *
 * Where class name is [a-z]+ and user agent string any string ended with
 * a new line character. The class name must have a white space character to
 * separate it from the user agent string.
 *
 * Returns the number of lines read from that file.
 */
int read_uas_with_class(const char *path, struct uas_record *root);

void print_uas_records(struct uas_record *root);

struct uas_record *uas_record_create();
void uas_record_free(struct uas_record *uas_record);
void uas_record_set(struct uas_record *uas_record, char *class, char *uas, struct uas_record *next);

#endif
