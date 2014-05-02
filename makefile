#
# Instructions
# Run 'make'
#

# Some general and some specific settings for the compiler and the linker
CC      = clang
CFLAGS  = -g
LDFLAGS = -L/usr/local/lib -I/usr/local/include -lmongoc

# Folder structure:
#   keep the binaries and the .o files in a separate directory
#
#   the binaries can then be easily excluded from the source code repository
#
BIN = ./bin
SRC = ./src
TST = ./test

PROG = $(BIN)/detection
HDRS = $(SRC)/ann.h $(SRC)/dbh.h
SRCS = $(addprefix $(SRC)/, detection.c ann.c dbh.c utils.c)
OBJS = $(SRCS:$(BIN)/.c=$(SRC)/.o)


all: $(SRCS) $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean :
	-rm -f core $(PROG) $(BIN)/*.o

#TAGS : $(SRCS) $(HDRS)
#	etags -t $(SRCS) $(HDRS)

$(BIN)/.c$(SRC)/.o :
	$(CC) $(CFLAGS) $< -o $@
