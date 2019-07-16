/*
====================
Daytime client protocol-dependent that  works with IPV6.
====================
*/
#include "unp.h"

int main( int argc, char **argv ) {
    int sockfd, n;
    char recvline[MAXLINE + 1] = {0};

    struct sockaddr_in6 servaddr;

    if ( argc != 2 ) {
        unp::ErrorQuit( "Usage: ch1_daytimetcpcliv6 <IPAddress>" ); 
    }

    if ( (sockfd = socket( AF_INET6, SOCK_STREAM, 0 )) < 0 ) {
        unp::ErrorSys( "Socket error" );
    }

#ifdef HAVE_STRINGS_H
    bzero( &servaddr, sizeof( servaddr ) );
#else
    memset( &servaddr, 0, sizeof( servaddr ) );
#endif

    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(13);      // need to convert to network byte order
                                        // network byte order is always in big-endian
    if ( inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0 ) {
        unp::ErrorQuit( "inet_pton error for %s", argv[1]);
    }

    if ( connect( sockfd, (const SA *)&servaddr, sizeof( servaddr ) ) == -1 ) {
        unp::ErrorSys( "Connect error" );
    }

    while ( (n = read( sockfd, recvline, MAXLINE )) > 0 ) {
        recvline[n] = 0;                // set null-terminated to read line

        if ( std::fputs( recvline, stdout ) == EOF ) {
            unp::ErrorSys( "fputs error" );
        }
    }

    if ( n < 0 ) {
        unp::ErrorSys( "read error" );
    }

    return 0;
}
