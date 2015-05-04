# This is the Makefile for the ua-detection project.
#
# Binary targets:
#
# * uadet: builds a binary into bin/uadet. This is the user-agent program 
#          that classifies user-agents.
#
# * uadb: builds a binary into bin/uadb. This is a helper utility which 
#         uses a file-backed storage for user-agent strings.
#
# ---
#
# For instruction on how to build the project see INSTALL file.
#

# Set the compiler
CC=gcc
# At the time OS X latest is the Yosemite version and it does not come with GCC.
# It comes with clang so if clang is available on the system, switch to that 
# compiler.
if hash clang 2>/dev/null; then \
	$(CC)=clang; \
fi

CFLAGS=-g -std=c99 -lm
INCDIRS=
SRCDIR=src
BINDIR=bin
TESTDIR=test
DEPS=

$(SRCDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#
# Binary targets
# 

UAPROGRAMSRC=$(addprefix $(SRCDIR)/, bitmask.c dictionary.c link_node.c probab.c program.c reader.c tokenizer.c utils.c)

uadet: $(UAPROGRAMSRC)
	@if [ ! -d bin ]; then \
	  mkdir bin; \
	fi
	$(CC) -o $(BINDIR)/uadet $^ $(CFLAGS) $(INCDIRS)


UADBSRC=$(addprefix $(SRCDIR)/, btree.c link_node.c db.c ua_db.c)

uadb: $(UADBSRC)
	@if [ ! -d bin ]; then \
	  mkdir bin; \
	fi
	$(CC) -o $(BINDIR)/uadb $^ $(CFLAGS) $(INCDIRS)
	

# AUXILLIARY

all: test ua_program ua_db

clean:
	rm -f $(SRCDIR)/*.o

test-clean:
	rm -f $(TESTDIR)/*.o

.PHONY: test clean

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
	$(CC) -g -c -o $@ $< $(CFLAGS)

test: $(addprefix $(TESTDIR)/, $(notdir $(TESTFILES:.c=.o)))
	$(CC) $(CFLAGS) -o $(TESTDIR)/$@ $^
	rm $(TESTDIR)/*.o
	test/test
	@if [ $$? -eq 0 ]; then \
	  echo "ALL TESTS PASSED"; \
	fi

#
# Make download-data an executable file by the user group
#

bashex:
	@chmod u+x data/download-data.sh
