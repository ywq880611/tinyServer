#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>

#include "block_queue.h"

class Log
{
private:
    Log(/* args */);
    ~Log();
    void async_write_log(){
        string string_log;
        while(m_log_queue->pop(string_log)){
            m_mutex.lock();
            fputs(string_log.c_str(), m_file);
            m_mutex.unlock();
        }
    }
    char dir_name[128];
    char log_name[128];
    int m_split_lines;
    int m_log_buf_size;
    long long m_count;
    int m_today;
    FILE *m_file;
    char *m_buf;
    block_queue<string> *m_log_queue;
    bool m_is_async;
    locker m_mutex;
    int m_close_log;

public:
    static Log *get_instance(){
        static Log instance;
        return &instance;
    }
    static void *flush_log_thread(void* args){
        Log::get_instance()->async_write_log();
    }

    bool init(const char*, int, int = 8192, int = 5000000, int = 0);
    void write_log(int, const char*, ...);
    void flush();
};

#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3, format, ##__VA_ARGS__); Log::get_instance()->flush();}


#endif