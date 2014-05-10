all: tests

tests: test_utils test_uas

test_utils:
	clang ../src/utils.c suite.c test_utils.c -o test_utils

test_uas:
	clang ../src/uas.c suite.c test_uas.c -o test_uas

test: tests
	./test_utils
	./test_uas