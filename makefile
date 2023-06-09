OBJS	= main.o pt_permutation.o dynamic_permutation.o sample.o
SOURCE	= main.cpp pt_permutation.cpp dynamic_permutation.cpp sample.cpp
HEADER	= pt_permutation.hpp dynamic_permutation.hpp sample.hpp
OUT	= a.exe
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: a.exe

run: a.exe
	./a.exe

a.exe: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


%.o: %.cpp $(HEADER)	
	$(CC) $(FLAGS) --compile $< --std=c++14 -o $@
 

clean:
	del *.o *.exe
