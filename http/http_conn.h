#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/mman.h>

class http_conn{
public:
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;

    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };


public:
    http_conn(){}
    ~http_conn(){}
    void init(int, struct sockaddr_in*, bool, bool);
    void close_conn();
    int read_once();
    void process();

private:
    void init();
    void addfd(bool, bool);
    void removefd();
    void modfd(int, bool);
    void setnonblocking(int);
    char *get_line() { return m_read_buf + m_start_line; };
    LINE_STATUS parse_line();
    HTTP_CODE process_read();
    HTTP_CODE parse_request_line(char*);
    HTTP_CODE parse_headers(char*);
    HTTP_CODE parse_content(char*);
    HTTP_CODE do_request();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();
    bool process_write(HTTP_CODE);
    
    

public:
    static int m_epollfd;

private:
    int m_sockfd;
    struct sockaddr_in *m_addr;
    bool m_is_oneshot;
    bool m_is_ET;
    char m_read_buf[READ_BUFFER_SIZE];
    int m_read_idx;
    char m_write_buf[WRITE_BUFFER_SIZE];
    int m_write_idx;
    int m_start_line;
    int m_checked_idx;
    int m_content_length;
    int bytes_to_send;
    int m_iv_count;
    bool m_linger;
    char* m_url;
    char* m_version;
    char* m_host;
    char* m_string;
    char* m_real_file;
    char* m_file_address;
    struct stat m_file_stat;
    struct iovec m_iv[2];
    METHOD m_method;
    CHECK_STATE m_check_state;
    

};


#endif