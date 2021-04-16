#ifndef LOCKER_H
#define LOCKER_H

#include <semaphore.h>
#include <pthread.h>
#include <exception>

class sem{
private:
    sem_t m_sem;   

public:
    sem();
    sem(int);
    ~sem();
    bool wait();
    bool post();
};

class locker{
private:
    pthread_mutex_t m_mutex;

public:
    locker();
    ~locker();
    bool lock();
    bool unlock();
    pthread_mutex_t *get();
};

class cond{
private:
    pthread_cond_t m_cond;

public:
    cond();
    ~cond();
    bool wait(locker*);
    bool signal_one();
    bool signal_all();
};


#endif