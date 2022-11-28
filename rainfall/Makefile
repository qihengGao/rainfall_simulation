#----- Modify CC and CFLAGS as appropriate in your environment
CPP = g++
CFLAGS = -O3 -std=c++11 -pthread -fopenmp

LIB = -lm -lc

PROGS = rainfall_seq rainfall_pt
OBJS = $(patsubst %, %.o, $(PROGS)) simulator.o
all: $(PROGS)

#------------ EXECUTABLE FILES ---------------
rainfall_seq: rainfall_seq.cpp simulator.o
	$(CPP) $(CFLAGS) -o rainfall_seq $^
# rainfall_pt: rainfall_pt.cpp
# 	$(CC) $(CFLAGS) -o rainfall_pt $^

.PHONY: clean
simulator.o: simulator.hpp
clean:
	rm -f $(PROGS) $(OBJS) *~