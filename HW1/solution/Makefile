output: main.o complexnumber.o
	g++ main.o complexnumber.o -o output
	
main.o: main.cpp
	g++ -c main.cpp
	
complexnumber.o: complexnumber.cpp complexnumber.h
	g++ -c complexnumber.cpp
	
clean:
	rm *.o output
