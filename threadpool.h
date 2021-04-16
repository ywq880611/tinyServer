#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>

template <typename T>
class threadpool{
    int m_thread_num;
    int m_max_jobs;
    pthread_t *m_threads;
    std::list<T*> m_jobs;

    threadpool();
    ~threadpool();
    bool append(T *request);

};

#endif