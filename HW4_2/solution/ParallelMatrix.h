#ifndef PARALLEL_MATRIX_H
#define PARALLEL_MATRIX_H

#include <pthread.h>

// the class of ParallelMatrix
class ParallelMatrix
{
private:
    // the matrix
    int **matrix;

    // the mutex for manageing parallel sum
    pthread_mutex_t sumMutex;

    // the sum counted by parallel counter
    int parallelSum;

    // the matrix height
    int height;

    // the matrix weight
    int weight;

public:
    // default constructor
    ParallelMatrix(int height, int weight);

    // initializes the matrix with random numbers
    void RandomInit();

    // calculates sum of all cells in the matrix
    int Sum();

    // parallely calculates sum of all cells in the matrix using threads
    int SumParallel(int threadsCount);

    // parallel sum thread function
    static void* ThreadFunction(void *arg);
};

#endif