#include "network/tcp_server.h"

int main() {
    TCPServer server;
    server.start(8080);
    return 0;
}