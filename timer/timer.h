#ifndef TIMER_H
#define TIMER_H

#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#include "../http/http_conn.h"

class util_timer;

struct client_data{
    int sockfd;
    struct sockaddr_in peer_addr;
    util_timer timer;
};


class util_timer{
public:
    util_timer(): prev(NULL), next(NULL){}
    ~util_timer(){}
    void (* cb_func)(struct client_data*);


    time_t expire;
    struct client_data* client;
    util_timer* prev;
    util_timer* next;
};

class sort_timer_list{
public:
    sort_timer_list():head(NULL), tail(NULL){}
    ~sort_timer_list();

    void add_timer(util_timer*);
    void adjust_timer(util_timer*);
    void del_timer(util_timer*);
    void tick();


private:
    void add_timer(util_timer*, util_timer*);
    util_timer* head;
    util_timer* tail;
};

class Utils{
public:
    Utils();
    Utils();

    void init(int);
    int setnonblocking(int);
    void addfd(int, int, bool, int);
    static void sig_handler(int );
    void addsig(int, void(handler)(int), bool = true);
    void timer_handler();
    void show_error(int, const char*);

    static int* u_ds;
    sort_timer_list m_timer_list;
    static int u_epollfd;
    int m_TIMESLOT;
};

void cb_func(client_data* cd);

#endif