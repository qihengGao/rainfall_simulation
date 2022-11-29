#----- Modify CC and CFLAGS as appropriate in your environment
CC = g++
# CFLAGS = -O3 -std=c++11 -pthread -fopenmp
CFLAGS = -O3 -std=c++11 -lpthread -fopenmp

PROGS = rainfall_seq rainfall_pt
OBJS = $(patsubst %, %.o, $(PROGS)) simulator.o simulatorpro.o barrier.o
all: $(PROGS)

#------------ EXECUTABLE FILES ---------------
rainfall_seq: rainfall_seq.o simulator.o
	$(CC) $(CFLAGS) -o $@ $^
rainfall_pt: rainfall_pt.o simulator.o simulatorpro.o barrier.o
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
simulator.o: simulator.hpp
simulatorpro.o: simulatorpro.hpp
barrier.o: barrier.hpp
.PHONY: clean
clean:
	rm -f $(PROGS) $(OBJS) *~