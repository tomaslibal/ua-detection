# 
# For instruction on how to build the project see INSTALL
#

CC=clang
CFLAGS=

SRCDIR=src
BINDIR=bin
TESTDIR=test

_DEPS=
_OBJ=program.o
OBJ=$(addprefix $(SRCDIR)/,$(_OBJ))

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#
# Main target ua_program
#
ua_program: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $< $(CFLAGS)
	rm $(SRCDIR)/*.o


# 
# U N I T    T E S T I N G
#
# Tests are bound together in test/main.c
#
# test/main.c specifies which tests will be run. When executing this make 
# target, all tests are rebuild and one executable test/test is created
# and subsequently run. If the exit code from test/test is 0, then all
# tests passed.
#
TESTFILES=$(wildcard $(TESTDIR)/*.c)

$(TESTDIR)/.o : $(TESTDIR)/.c
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(addprefix $(TESTDIR)/, $(notdir $(TESTFILES:.c=.o)))
	$(CC) -o $(TESTDIR)/$@ $^
	rm $(TESTDIR)/*.o
	test/test
	@if [ $$? -eq 0 ]; then \
	  echo "ALL TESTS PASSED"; \
	fi

.PHONY: test

#
#
#

bashex:
	@chmod u+x data/download-data.sh
