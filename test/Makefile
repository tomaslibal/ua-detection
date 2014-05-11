all: tests

tests: test_utils test_uas test_dbh

test_utils:
	clang ../src/utils.c suite.c test_utils.c -o test_utils

test_uas:
	clang ../src/uas.c suite.c test_uas.c -o test_uas

test_dbh:
	clang ../src/dbh.c suite.c test_dbh.c -o test_dbh -L/usr/local/lib -I/usr/local/include -lmongoc

test: tests
	@err=0;
	@for f in ./test_utils ./test_uas ./test_dbh ; \
	do \
		$$f; \
	done
	@echo "*** OK *** The suite passed"
	@rm ./test_utils ./test_uas ./test_dbh

clean:
	rm ./test_utils ./test_uas ./test_dbh
