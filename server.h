#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1024


class Server{
    int m_port;
    int m_listenfd;
    char m_buf[BUFFER_SIZE];

    public:
        Server(int);
        ~Server();
        int run();
};