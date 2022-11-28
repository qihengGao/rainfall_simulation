#----- Modify CC and CFLAGS as appropriate in your environment
CC = g++
# CFLAGS = -O3 -std=c++11 -pthread -fopenmp
CFLAGS = -std=c++11 -pthread -fopenmp

PROGS = rainfall_seq rainfall_pt
OBJS = $(patsubst %, %.o, $(PROGS)) simulator.o
all: $(PROGS)

#------------ EXECUTABLE FILES ---------------
rainfall_seq: rainfall_seq.o simulator.o
	$(CC) $(CFLAGS) -o $@ $^
rainfall_pt: rainfall_pt.o simulator.o
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<
simulator.o: simulator.hpp

.PHONY: clean
clean:
	rm -f $(PROGS) $(OBJS) *~