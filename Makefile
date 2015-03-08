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

ua_program: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $< $(CFLAGS)
	rm $(SRCDIR)/*.o

# 
# U N I T    T E S T I N G
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
