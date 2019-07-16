/*
====================
Daytime TCP server
====================
*/
#include "unp.h"
#include <ctime>
#include <sstream>

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    std::ostringstream buff;
    std::time_t ticks;

    listenfd = unp::Socket(AF_INET, SOCK_STREAM, 0);

    std::memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    unp::Bind(listenfd, (const SA *)&servaddr, sizeof(servaddr));
    unp::Listen(listenfd, LISTENQ);

    std::cout << "listening..." << std::endl;

    while (true) {
        connfd = unp::Accept(listenfd, NULL, NULL);

        ticks = std::time(nullptr);
        buff << std::ctime(&ticks) << "\r\n";
        const std::string& tmpBuff = buff.str();        // remember: str() creates a temporary object
        unp::Write(connfd, tmpBuff.c_str(), tmpBuff.length());
        unp::Close(connfd);
    }

    return 0;
}
