#include "PipeCalculator.h"
#include <sys/wait.h>
#include <iostream>
//#include <unistd.h>
#include <stdlib.h>
#include <cerrno>
#include <map>
#include <math.h>
#include <cstring>

PipeCalculator::PipeCalculator(int arraySize)
{
    // initialize
    this->arraySize = arraySize;
    this->array = new int[this->arraySize];

    // initialize array with random numbers
    for (int i = 0; i < this->arraySize; ++i)
    {
        this->array[i] = rand() % 10;
        std::cout << this->array[i] << " ";
    }

    std::cout << std::endl;
}

// calculate the sum of array parallely using pipe
int PipeCalculator::sum(int workersCount)
{
    // get range of indexes os sub-total
    int indexRange = ceil((float)this->arraySize / workersCount);

    // the total sum to be calculated
    int totalSum = 0;

    // create workers
    for (int i = 0; i < workersCount; ++i)
    {
        // create array to keep pipe descriptors
        int parentToChildPipefd[2];
        int childToParentPipefd[2];

        // create pipes
        int parentToChildPipeResult = pipe(parentToChildPipefd);
        int childToParentPipeResult = pipe(childToParentPipefd);

        // check if failed
        if (parentToChildPipeResult != 0)
        {
            exit(errno);
        }

        // check if failed
        if (childToParentPipeResult != 0)
        {
            exit(errno);
        }

        // create a process
        int child = fork();

        // exit on failure
        if (child == -1)
        {
            exit(errno);
        }

        // parent process
        if (child > 0)
        {
            // calculate 'from' and 'to' indexes
            int fromIndex = i * indexRange;
            int toIndex = i * indexRange + indexRange;

            // reach the end
            if (i == workersCount - 1)
            {
                toIndex = this->arraySize;
            }

            // store indexes
            int *indexes = new int[2];
            indexes[0] = fromIndex;
            indexes[1] = toIndex;

            // close unused read end of parentToChildPipefd
            close(parentToChildPipefd[0]);

            // close unused write end of childToParentPipefd
            close(childToParentPipefd[1]);

            int written = write(parentToChildPipefd[1], indexes, sizeof(indexes));

            // make sure data sent
            if (written == -1)
            {
                std::cout << std::strerror(errno);
                exit(0);
            }

            // create buffer to write the bytes read
            int childCountedSum = 0;

            // read from pipe
            int readBytes = read(childToParentPipefd[0], &childCountedSum, sizeof(childCountedSum));

            // std::cout << "childCountedSum " << childCountedSum << std::endl;

            totalSum += childCountedSum;
        }
        // child process
        else if (child == 0)
        {
            int *indexes = new int[2];

            // close unused write end of parentToChildPipefd
            close(parentToChildPipefd[1]);

            // close unused read end of childToParentPipefd
            close(childToParentPipefd[0]);

            // read bytes from pipe
            int readBytes = read(parentToChildPipefd[0], indexes, sizeof(indexes));

            int from = indexes[0];
            int to = indexes[1];

            // calculate sub-total sum
            int sum = 0;
            for (int j = from; j < to; ++j)
            {
                sum += array[j];
            }

            // print sum
            std::cout << "sum from " << from << " to " << to << " is " << sum << std::endl;

            // send sum to parent
            int written = write(childToParentPipefd[1], &sum, sizeof(childToParentPipefd));

            exit(0);
        }

        // wait for all workers to finish
    }
    while (wait(NULL) > 0);
    return totalSum;
}

PipeCalculator::~PipeCalculator()
{
    delete[] this-> array;
    delete[] this-> parallelWorkers;
}