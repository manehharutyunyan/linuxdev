all: PipeCalculator

PipeCalculator: main.o PipeCalculator.o
	g++ -pthread main.o PipeCalculator.o -o PipeCalculator
	
main.o: main.cpp PipeCalculator.h
	g++ -c main.cpp -o main.o

PipeCalculator.o: PipeCalculator.cpp
	g++ -c PipeCalculator.cpp

clean:
	rm *.o PipeCalculator