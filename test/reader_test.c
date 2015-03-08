#include <assert.h>
#include <string.h>

#include "../src/reader.c"

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

	print_uas_records(root);

	assert(lc == 2);
}

void run_test_reader()
{
	test_reader_file_with_class();
}
