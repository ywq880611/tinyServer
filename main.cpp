#include "server.h"

int main(int argc, char **argv){
    Server echo = Server(8080);
    echo.run();
}