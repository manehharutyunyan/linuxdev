The problem:
Implement a class called "ParallelMatrix" to model the matrix data structure.

Behavior: 
- Class has a method called Sum and SumParallel
- Class has Init method that initializes all the cells with random integers
- Sum method calculates sum of all cells in the matrix
- SumParallel calculates the sum of all cells using threads (method accepts number of threads)
- Using pthread for creating/waiting threads
- Every thread should take care of one of sub-matricies of the matrix. 
- Both methods are returning sum of all elements.

Write a main function that creates and initializes all the cells and calculates the sum. Measure the times of Sum, SumParallel(4), SumParallel(8), SumParallel(16), SumParallel(100).

Class should be implemented with accurate .cpp/.h files separately and main.cpp should be a separate module.
Write a Makefile to build the program.