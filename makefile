CC = clang
CFLAGS = -g
LDFLAGS = 

# Folder structure:
#   keep the binaries and the .o files in a separate directory
#   
BIN = ./bin
SRC = ./src

PROG = $(BIN)/detection
HDRS = $(SRC)/ann.h
SRCS = $(SRC)/detection.c $(SRC)/ann.c
OBJS = $(BIN)/detection.o $(BIN)/ann.o

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