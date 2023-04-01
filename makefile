OBJS	= main.o pt_permutation.o sample.o
SOURCE	= main.cpp pt_permutation.cpp sample.cpp
HEADER	= pt_permutation.hpp sample.hpp
OUT	= a.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

#main.o: main.cpp
	#$(CC) $(FLAGS) main.cpp -std=c++14

#pt_permutation.o: pt_permutation.cpp
	#$(CC) $(FLAGS) pt_permutation.cpp -std=c++14

%.o: %.cpp $(HEADER)	
	$(CC) $(FLAGS) --compile $< --std=c++14 -o $@
 

clean:
	rm -f $(OBJS) $(OUT)
