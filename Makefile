CC = gcc
DEBUG = -g
CFLAGS?= $(DEBUG) -Wall -D_FILE_OFFSET_BITS=64 -pthread 
LIBS =  -lm -lz -lpthread
BIN = new2old 
sm_obj = new2old.o  parse.o  sv.o  sam.o tras.o  check.o
all:$(BIN)

new2old:$(sm_obj)
	$(CC) -o  $@ $^ $(LIBS)	

clean:
	rm $(BIN) *o  *~
