SRC = llord.cpp
OBJ = $(SRC:.cpp = .o)
PROGRAM = ../llord_clustering
CC = g++
CFLAGS = -std=c++11 -g
all:$(PROGRAM)

$(PROGRAM):$(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ)

SUFFIXES: .cpp

cpp.o:
	$(CC) $(CFLAGS) -c $<
clean:
	-rm -rf *.o $(PROGRAM)
