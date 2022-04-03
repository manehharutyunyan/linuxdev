#include <iostream>
#include "ParallelMatrix.h"
#include <chrono>

int main()
{
    ParallelMatrix matrix = ParallelMatrix(14, 4);
    matrix.RandomInit();

    // start time of sequential sum
    auto startedSequentialTime = std::chrono::high_resolution_clock::now();

    // get the sequentially counted sum of matrix
    std::cout << "sequential sum: " << matrix.Sum() << std::endl;

    // end time of sequentially sum
    auto endSequentialTime = std::chrono::high_resolution_clock::now();

    // start time of parallel sum
    auto startedParallelTime = std::chrono::high_resolution_clock::now();

    // get the parallelly counted sum of matrix
    std::cout << "parallel sum: " << matrix.SumParallel(11) << std::endl;

    // end time of parallel sum
    auto endParallelTime = std::chrono::high_resolution_clock::now();

    std::cout << "duration of sequentially sum is: " << std::chrono::duration_cast<std::chrono::milliseconds>(endSequentialTime-startedSequentialTime).count() << std::endl;
    std::cout << "duration of paralel sum is: " << std::chrono::duration_cast<std::chrono::milliseconds>(endParallelTime-startedParallelTime).count() << std::endl;
}