#include "server.h"

Server::Server(int port){
    m_port = port;
}

Server::~Server(){

}

int Server::run(){
    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd > 0);

    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(m_port);

    ret = bind(m_listenfd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret >= 0);

    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    while(true){
        struct sockaddr_in peer_addr;
        socklen_t peer_len = 0;
        int connfd = accept(m_listenfd, (struct sockaddr *)&peer_addr, &peer_len);
        if(fork()){
            //parent
        }
        else{
            //children
            memset(m_buf, 0, BUFFER_SIZE);
            int len;
            while(len = read(connfd, m_buf, BUFFER_SIZE) > 0){
                write(connfd, m_buf, len);
                printf("echo: %s\n", m_buf);
            }
            _exit(0);
        }
    }
}