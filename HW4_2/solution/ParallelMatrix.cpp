#include "ParallelMatrix.h"
#include <stdlib.h>
#include <iostream>
#include <cmath>

// initialize parallel matrix
ParallelMatrix::ParallelMatrix(int height, int weight)
{
    this->height = height;
    this->weight = weight;
}

// initialize matrix with random numbers
void ParallelMatrix::RandomInit()
{
    this->matrix = new int *[this->height];
    pthread_mutex_init(&this->sumMutex,NULL);

    for (int i = 0; i < this->height; ++i)
    {
        this->matrix[i] = new int[weight];
        for (int j = 0; j < this->weight; ++j)
        {
            this->matrix[i][j] = rand() % 10;
        }
    }

    // make matrix flatten
    this->flattenMatrix = new int[this->height * this->weight];
    for(int i = 0; i < this->height; ++i)
    {
        for(int j = 0; j < this->weight; ++j)
        {
            this->flattenMatrix[i*this->weight + j] = this->matrix[i][j];
        }
    }

    for (int i = 0; i < this->height; ++i)
    {
        for (int j = 0; j < this->weight; ++j)
        {
            //std::cout << this->matrix[i][j] << " ";
        }
        //std::cout << std::endl;
    }
}

// sequentially calculate the sum of matrix elements
int ParallelMatrix::Sum()
{
    // the sum
    int sum = 0;

    // calculate the sum of all cells
    for (int i = 0; i < this->height; ++i)
    {
        for (int j = 0; j < this->weight; ++j)
        {
            sum += matrix[i][j];
        }
    }

    return sum;
}

// the thread arguments structure
struct thread_args
{
    ParallelMatrix *instance;
    int rowStartIndex;
    int rowEndIndex;
};

// calculate the sum of cells in submatrix
void *ParallelMatrix::ThreadFunction(void *arg)
{
    int currSum = 0;

    // get thread argument
    struct thread_args *args = (struct thread_args *)arg;

    // get parallel matrix instance
    ParallelMatrix *instance = (ParallelMatrix *)args->instance;

    for (int i = args->rowStartIndex; i < args->rowEndIndex; ++i)
    {
        currSum += instance->flattenMatrix[i];
    }

    // lock mutex
    pthread_mutex_lock(&instance->sumMutex);

    // securely add curr sum to main parallelSum
    instance->parallelSum += currSum;

    // unlock mutex
    pthread_mutex_unlock(&instance->sumMutex);

    return NULL;
}

// parallelly calculates the sum of matrix elements
int ParallelMatrix::SumParallel(int threadsCount)
{
    // int sum = 0;
    this->parallelSum = 0;

    pthread_t threads[threadsCount];

    // the size of flatten matrix
    int flattenMatrixSize = this->height*this->weight;

    // get the count of submatrix rows
    //int submatrixRowsCount = ceil((float)this->height / threadsCount);
    int capacity = ceil((float)flattenMatrixSize/threadsCount);

    // for each iteration create an thread
    for (int i = 0; i < threadsCount; ++i)
    {
        // get end index of submatrix
        int endIndex = i * capacity + capacity;

        // reach the end
        if (i == threadsCount - 1)
        {
            endIndex = flattenMatrixSize;
        }

        // initialize thread struct
        thread_args *args = new (thread_args);
        args->rowStartIndex = i * capacity;
        args->rowEndIndex = endIndex;
        args->instance = this;

        // create thread to run threadFunction routine
        int threadCreated = pthread_create(&threads[i], NULL, ThreadFunction, args);

        // check if created succesfuly
        if (threadCreated != 0)
        {
            std::cout << "Could not create new thread" << std::endl;
            return threadCreated;
        }
    }

    // wait for all threads
    for (int i = 0; i < threadsCount; i++)
    {
        // return value of thread
        void *retval;

        // join the thread and get its return value
        pthread_join(threads[i], &retval);
    }

    return this->parallelSum;
}
