CC = gcc
DEBUG = -g
CFLAGS?= $(DEBUG) -Wall -D_FILE_OFFSET_BITS=64 -pthread 
LIBS =  -lm -lz -lpthread
BIN = new2old extract  breaksam
sm_obj = new2old.o  parse.o  sv.o  sam.o tras.o  check.o 
ext_obj = extract.o
break_obj = breaksam.o sam.o  check.o  parse.o
all:$(BIN)

new2old:$(sm_obj)
	$(CC) -o  $@ $^ $(LIBS)	

extract:$(ext_obj)
	$(CC) -o  $@ $^ $(LIBS)

breaksam:$(break_obj)
	$(CC) -o  $@ $^ $(LIBS)

clean:
	rm $(BIN) *o  *~
