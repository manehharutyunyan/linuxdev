all: ParallelScheduler

ParallelScheduler: main.o ParallelScheduler.o
	g++ -pthread main.o ParallelScheduler.o -o ParallelScheduler
	
main.o: main.cpp ParallelScheduler.h
	g++ -c main.cpp -o main.o

ParallelScheduler.o: ParallelScheduler.cpp
	g++ -c ParallelScheduler.cpp

clean:
	rm *.o ParallelScheduler