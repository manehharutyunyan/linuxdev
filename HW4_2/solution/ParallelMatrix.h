#ifndef PARALLEL_MATRIX_H
#define PARALLEL_MATRIX_H

#include <pthread.h>

// the class of ParallelMatrix
class ParallelMatrix
{
private:
    // the matrix
    int **matrix;

    // the flatten matrix
    int *flattenMatrix;

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

    // make matrix flatten
    void FlattenMatrix();

    // calculates sum of all cells in the matrix
    int Sum();

    // parallely calculates sum of all cells in the matrix using threads
    int SumParallel(int threadsCount);

    // securely add 'sum' to total 'parallelSum'
    void AddToParallelSum(int sum);

    // parallel sum thread function
    static void* ThreadFunction(void *arg);
};

#endif