#include <assert.h>
#include <string.h>

#include "../src/reader.c"

#include "test_suite.h"

int i_reader = 0;

void test_reader_file_with_class()
{
	struct uas_record *root = NULL;
	root = uas_record_create();
	if (root == NULL) {
		exit(1);
	}
	// line count
	int lc = 0;
	// cwd is the repo root as the test is run from Make utility
	// #include <unistd.h>
	// char cwd[512];
	// getcwd(cwd, sizeof(cwd));
	lc = read_uas_with_class("test/resources/uas_with_class.txt", root);

	assert(lc == 2);

	uas_record_free(root);

	success(i_reader);
}

int run_test_reader()
{
	test_reader_file_with_class();

	return i_reader;
}
