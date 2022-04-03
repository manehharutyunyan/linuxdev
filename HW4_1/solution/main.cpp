#include "ParallelScheduler.h"
#include <iostream>
#include <unistd.h>

void f(void* arg)
{
    int* i = (int*)arg;

    std::cout << "Thread " << gettid() << " is working on " << *i << std::endl;

    delete i;
}

int main()
{
    ParallelScheduler* scheduler = new ParallelScheduler(10);

    for(int i = 0; i < 10000; ++i)
    {
        scheduler->run(f, new int(i));
    }

    while(true)
    {
        sleep(2);
        std::cout << "Main is working" << std::endl;
    }

    delete scheduler;
    return 0;
}