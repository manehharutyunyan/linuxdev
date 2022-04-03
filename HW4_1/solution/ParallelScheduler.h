#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>
#include <utility>

typedef void (*scheduler_fn_t) (void*);

class ParallelScheduler
{
    private:
        int capacity;
        pthread_t* threads;
        std::queue<std::pair<scheduler_fn_t, void*>> functions;
        pthread_mutex_t* queueLock;
        pthread_cond_t* hasFunction;

    private:
        static void* execute(void* arg);

    public:
        ParallelScheduler(int capacity);
        ~ParallelScheduler();
        void run(scheduler_fn_t func, void* arg);
};

#endif