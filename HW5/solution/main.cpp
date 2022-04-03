#include "PipeCalculator.h"
#include <iostream>

int main()
{
    PipeCalculator* pipeCalculator = new PipeCalculator(10);

    std::cout << "total sum = " << pipeCalculator->sum(3) << std::endl;

    return 0;
}