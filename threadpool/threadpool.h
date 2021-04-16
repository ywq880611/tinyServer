#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <deque>
#include <pthread.h>
#include "../lock/locker.h"


template<typename T>
class threadpool
{
private:
    locker m_locker;
    cond m_cond;
    //sem m_sem;
    int m_max_threads;
    int m_max_jos;
    pthread_t m_threads[];
    std::deque<T*> m_tasks;

public:
    threadpool(int = 8, int = 10000);
    ~threadpool();
    bool appendTask(T *task);
private:
    static void *worker(void*);
    void run();
};

#endif