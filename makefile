CC = clang
CFLAGS = -g `pkg-config --cflags --libs libmongoc-1.0`
LDFLAGS =

# Folder structure:
#   keep the binaries and the .o files in a separate directory
#
BIN = ./bin
SRC = ./src

PROG = $(BIN)/detection
HDRS = $(SRC)/ann.h $(SRC)/dbh.h
SRCS = $(SRC)/detection.c $(SRC)/ann.c $(SRC)/dbh.c
OBJS = $(BIN)/detection.o $(BIN)/ann.o $(BIN)/dbh.o

# LIST = $(addprefix $(BIN)/, $(PROG))

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(LDFLAGS) $(OBJS)

clean :
	rm -f core $(PROG) $(OBJS)

TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)

$(BIN)/detection.o : $(SRC)/detection.c
	$(CC) $(CFLAGS) -c $(SRC)/detection.c -o $(BIN)/detection.o
$(BIN)/ann.o : $(SRC)/ann.c
	$(CC) $(CFLAGS) -c $(SRC)/ann.c -o $(BIN)/ann.o
$(BIN)/dbh.o : $(SRC)/dbh.c
	$(CC) $(CFLAGS) -c $(SRC)/dbh.c -o $(BIN)/dbh.o
