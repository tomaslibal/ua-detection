/*
  A simple reader utility to read ascii files
*/

#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_uas_with_class(const char *path, struct uas_record *root)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		return -1;
	}

	// tracks how many lines have been read
	int cnt = 0;
	// assume that the class name is not more than 32 chars
	char class_name[32];
	// assume that the user-agent string is not more than 512 chars
	char ua_string[512];

	struct uas_record *curr = root;
	struct uas_record *next = NULL;

	while(fscanf(fp, "%s %512[^\t\n]", class_name, ua_string) == 2) {
		//printf("class: %s, user-agent-string: %s\n", class_name, ua_string);
		next = uas_record_create();
		uas_record_set(curr, class_name, ua_string, next);
		curr = next;
		cnt++;
	}

	fclose(fp);

	return cnt;
}

void print_uas_records(struct uas_record *root)
{
	struct uas_record *curr = root;

	if (curr == NULL) {
		return;
	}

	while(curr->next != NULL) {
		printf("UAS_RECORD: class: %s, user-agent-string: %s\n", curr->class, curr->uas);
		curr = curr->next;
	}

}

struct uas_record *uas_record_create()
{
	struct uas_record *uas_record = NULL;
	uas_record = malloc(sizeof(struct uas_record));
	if (uas_record == NULL) {
		return NULL;
	}

	uas_record->class = NULL;
	uas_record->uas = NULL;
	uas_record->next = NULL;

	return uas_record;
}

void uas_record_free(struct uas_record *uas_record)
{
	if (uas_record == NULL) {
		return;
	}

	struct uas_record *tmp = NULL;
	struct uas_record *tmp_prev = NULL;

	tmp = uas_record;

	while(tmp != NULL) {
		if (tmp->uas != NULL) {
			free(tmp->uas);
		}
		if (tmp->class != NULL) {
			free(tmp->class);
		}
		tmp_prev = tmp;
		tmp = tmp->next;

		free(tmp_prev);
	}
}

void uas_record_set(struct uas_record *uas_record, char *class, char *uas, struct uas_record *next)
{
	if (uas_record == NULL) {
		return;
	}

	if (uas_record->class != NULL) {
		free(uas_record->class);
	}
	
	uas_record->class = malloc(sizeof(char) * strlen(class) + 1);
	
	strcpy(uas_record->class, class);

	if (uas_record->uas != NULL) {
		free(uas_record->uas);
	}
	
	uas_record->uas = malloc(sizeof(char) * strlen(uas) + 1);
	
	strcpy(uas_record->uas, uas);

	uas_record->next = next;
}

/*
int main()
{
    FILE *fp = fopen("../data/user_agents.txt", "r");
    if(fp == NULL) {
        exit(EXIT_FAILURE);
    }

    // keeps the count of how many user-agents have been read
    int counter = 0;
    // data column reserved for future use
    int n1;
    // data column reserved for future use
    int n2;
    // buffer for the user-agent strings
    char ua_string[256];
    while(fscanf(fp, "%d %d %256[^\t\n]", &n1, &n2, &ua_string) == 3) {
        printf("user-agent string = %s\n", ua_string);
        counter++;
    }

    printf("Done\nRead %d user-agent values\n", counter);

    fclose(fp);
    return 0;
}
*/
