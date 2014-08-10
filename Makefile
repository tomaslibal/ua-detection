CC=clang
CFLAGS=

SRCDIR=src
BINDIR=bin

_DEPS=
_OBJ=program.o
OBJ=$(addprefix $(SRCDIR)/,$(_OBJ))

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ua_program: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $< $(CFLAGS)
	rm $(SRCDIR)/*.o