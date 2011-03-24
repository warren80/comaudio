#include <QString>
#include <errno.h>

#ifndef _WIN32
#include <sys/socket.h>
#endif

#include "socket.h"

Socket::Socket(NetMode mode) : mode_(mode) {

    if ((socket_ = socket(PF_INET, mode_, IPPROTO_TCP)) == -1) {
        QString exception("error creating socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

void Socket::bind(int port) {
    sockaddr_in bindTo;
    memset((char*) &bindTo, 0, sizeof(bindTo));
    bindTo.sin_family = AF_INET;
    bindTo.sin_port = htons(port);
    bindTo.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(socket_, (sockaddr*) &bindTo, sizeof(bindTo)) == -1) {
        QString exception("error binding server socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

void Socket::listen(int backlog) {
    if (::listen(socket_, backlog) == -1) {
        QString exception("error listening on server socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

int Socket::receive(char* buffer, int length) {
    int read = 0;

    while ((read = recv(socket_, buffer, length, 0)) != -1) {
       buffer += read;
       length -= read;

       if (length == 0) {
           break;
       }
   }

   return read;
}

int Socket::transmit(char *buffer, int length) {
    return send(socket_, buffer, length, 0);
}

Socket Socket::accept() {
    int newSocket;
    sockaddr_in info;
    socklen_t len = sizeof(info);
    newSocket = ::accept(socket_, (sockaddr*) &info, &len);
    return Socket(newSocket, mode_, info);
}
