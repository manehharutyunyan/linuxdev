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

    for (int i = 0; i < this->height; ++i)
    {
        this->matrix[i] = new int[weight];
        for (int j = 0; j < this->weight; ++j)
        {
            this->matrix[i][j] = rand() % 10;
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
        for (int j = 0; j < instance->weight; ++j)
        {
            currSum += instance->matrix[i][j];
        }
    }

    // securely add current sum to total sum
    instance->AddToParallelSum(currSum);

    return NULL;
}

// securely add current sum to total sum
void ParallelMatrix::AddToParallelSum(int sum)
{
    // lock mutex
    pthread_mutex_lock(&this->sumMutex);

    // securely add curr sum to main parallelSum
    this->parallelSum += sum;

    // unlock mutex
    pthread_mutex_unlock(&this->sumMutex);
}

// parallelly calculates the sum of matrix elements
int ParallelMatrix::SumParallel(int threadsCount)
{
    this->parallelSum = 0;

    // logic doesn't work when threadsCount < this->height
    if (threadsCount >= this->height)
    {
        threadsCount = this->height;
    }

    // int sum = 0;
    this->parallelSum = 0;

    pthread_t threads[threadsCount];

    // get the count of submatrix rows
    int submatrixRowsCount = ceil((float)this->height / threadsCount);

    // for each iteration create an thread
    for (int i = 0; i < threadsCount; ++i)
    {
        // get end index of submatrix
        int endIndex = i * submatrixRowsCount + submatrixRowsCount;

        if (i == threadsCount - 1)
        {
            endIndex = this->height;
        }

        // initialize thread struct
        thread_args *args = new (thread_args);
        args->rowStartIndex = i * submatrixRowsCount;
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
