/*
====================
Provide wrapper function over standard functions used in network programming.

It usually call one of unp::Error... variants to handle error globally but provide a way
for user to handle before exiting or aborting the process.
====================
*/
#ifndef __WRAPPER_H_
#define __WRAPPER_H_

#include "unp_wrapped.h"
#include "Error.h"

namespace unp {
    /**
     * Create a socket via socket() and handle error via unp::ErrorSys() if there's error unless manualErrorHandler is set.
     */
    int         Socket(const int domain, const int type, const int protocol, const bool manualErroHandler=false);

    /**
     * Bind a name to socket via bind(), and handle error via unp::ErrorSys() if there's error unless manualErrorHandler is set.
     */
    int         Bind(const int sockfd, const SA *addr, const socklen_t addrlen, const bool manualErroHandler=false);
    
    /**
     * Listens for connection on a socket via listen() and handle error via unp::ErrorSys() if there's error unless manualErrorHandler is set.
     */
    int         Listen(const int sockfd, const int backlog, const bool manualErrorHandler=false);

    /**
     * Accept a connection on a socket via accept(), and handle error via unp::ErrorSys().
     */
    int         Accept(const int sockfd, SA *addr, socklen_t *addrlen, const bool manualErrorHandler=false);
    
    /**
     * Write to a file descriptor.
     */
    ssize_t     Write(int fd, const void *buf, const size_t count, const bool manualErrorHandler=false); 

    /**
     * Close a file descriptro.
     */
    int         Close(int fd, const bool manualErrorHandler=false);
};

inline int unp::Socket(const int domain, const int type, const int protocol, const bool manualErrorHandler) {
    int sockfd = socket(domain, type, protocol);
    if (sockfd < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("socket error");
        }
        return sockfd;  // should not get here
    }
    return sockfd;
}

inline int unp::Bind(const int sockfd, const SA *addr, const socklen_t addrlen, const bool manualErrorHandler) {
    int ret = bind(sockfd, addr, addrlen);
    if (ret < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("bind error");
        }
        return ret;
    }
    return ret;
}

inline int unp::Listen(const int sockfd, const int backlog, const bool manualErrorHandler) {
    int ret = listen(sockfd, backlog);
    if (ret < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("listen error");
        }
        return ret;
    }
    return ret;
}

inline int unp::Accept(const int sockfd, SA *addr, socklen_t *addrlen, const bool manualErrorHandler) {
    int connfd = accept(sockfd, addr, addrlen);
    if (connfd < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("accept error");
        }
        return connfd;
    }
    return connfd;
}

inline ssize_t unp::Write(int fd, const void *buf, const size_t count, const bool manualErrorHandler) {
    ssize_t s = write(fd, buf, count);
    if (s < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("write error");
        }
        return s;
    }
    return s;
}

inline int unp::Close(int fd, const bool manualErrorHandler) {
    int ret = close(fd);
    if (ret < 0) {
        if (!manualErrorHandler) {
            unp::ErrorSys("close error");
        }
        return ret;
    }
    return ret;
}

#endif /* __WRAPPER_H_ */
