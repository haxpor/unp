#include "unp.h"

int main( int argc, char **argv ) {
    // this program is not daemon
    // set to 0 to let it know
    unp::daemonProc = 0;

    int sockfd, n;
    char recvline[MAXLINE + 1];

    struct sockaddr_in servaddr;

    if ( argc != 2 ) {
        unp::ErrorQuit( "Usage: ch1_daytimeclient <IPAddress>" ); 
    }

    if ( (sockfd = socket( AF_INET, SOCK_STREAM, 0 )) < 0 ) {
        unp::ErrorSys( "Socket error" );
    }

#ifdef HAVE_STRINGS_H
    bzero( &servaddr, sizeof(servaddr) );
#else
    memset( &servaddr, 0, sizeof(servaddr) );
#endif

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);      // need to convert to network byte order
                                        // network byte order is always in big-endian
    if ( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0 ) {
        unp::ErrorQuit( "Connect error" );
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
