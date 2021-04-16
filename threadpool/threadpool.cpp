#include "threadpool.h"

template <typename T>
threadpool<T>::threadpool(int max_threads, int max_jobs) : m_max_threads(max_threads), m_max_jos(max_jobs){
    if(max_threads <= 0 || max_jobs <= 0) throw std::exception();
    m_threads = new pthread_t[max_threads];
    if(!m_threads) throw std::exception();
    for(int i = 0; i < m_max_threads; i ++){
        if(pthread_create(m_threads + i, NULL, worker, this) != 0){
            delete[] m_threads;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i]) != 0){
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template <typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
}

template <typename T>
bool threadpool<T>::appendTask(T *task){
    m_locker.lock();
    if(m_tasks.size() >= m_max_jos){
        m_locker.unlock();
        return false;
    }
    m_tasks.emplace_back(task);
    m_locker.unlock();
    m_cond.signal_one();
    //m_sem.post();
    return true;
}

template <typename T>
void* threadpool<T>::worker(void* args){
    threadpool *pool = reinterpret_cast<threadpool*>(args);
    pool->run();
    return pool; // it's hard to return ?
}

template <typename T>
void threadpool<T>::run(){
    T *task = NULL;
    while(true){
        m_locker.lock();
        while(m_tasks.empty()){
            m_cond.wait(&m_locker);
        }
        task = m_tasks.front();
        m_tasks.pop_front();
        m_locker.unlock();
        if(!task) continue;
        task();
    }
}