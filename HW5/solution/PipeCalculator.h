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

    //int (*parentChildFileDes)[2];
    //int (*childParentFileDes)[2];
    //int parentToChildFd[2];
    //int childToParentFd[2];

public:
    PipeCalculator(int arraySize);
    //~PipeCalculator();
    int sum(int parallelWorkersCount);
};

#endif