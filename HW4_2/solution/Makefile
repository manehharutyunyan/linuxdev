all: ParallelMatrix

ParallelMatrix: main.o ParallelMatrix.o
	g++ -pthread main.o ParallelMatrix.o -o ParallelMatrix
	
main.o: main.cpp ParallelMatrix.h
	g++ -c main.cpp -o main.o

ParallelMatrix.o: ParallelMatrix.cpp
	g++ -c ParallelMatrix.cpp

clean:
	rm *.o ParallelMatrix