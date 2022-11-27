#----- Modify CC and CFLAGS as appropriate in your environment
CC = g++
CFLAGS = -O3 -std=gnu++11 -Wall -pedantic -pthread -fopenmp

LIB = -lm -lc

PROGS = rainfall_seq rainfall_pt
OBJS = $(patsubst %, %.o, $(PROGS))
all: $(PROGS)

#------------ EXECUTABLE FILES ---------------
rainfall_seq: rainfall_seq.cpp
	$(CC) $(CFLAGS) -o rainfall_seq rainfall_seq.cpp

rainfall_pt: rainfall_pt.cpp
	$(CC) $(CFLAGS) -o rainfall_pt rainfall_pt.cpp

.PHONY: clean
clean:
	rm -f $(PROGS) $(OBJS) *~
