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
    return 0;
}
