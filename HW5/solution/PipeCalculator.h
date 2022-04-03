#ifndef PIPE_CALCULATOR_H
#define PIPE_CALCULATOR_H

#include <unistd.h> 

// the class of PipeCalculator
class PipeCalculator
{
private:
    int arraySize;

    int* array;
    pid_t *parallelWorkers;

public:
    PipeCalculator(int arraySize);
    ~PipeCalculator();
    int sum(int parallelWorkersCount);
};

#endif