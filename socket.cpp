#include <QString>
#include <errno.h>

#ifndef _WIN32
#include <sys/socket.h>
#endif

#ifdef _WIN32
#define SHUT_RDRW 2
#endif

#include "socket.h"

Socket::Socket(NetMode mode) : mode_(mode) {

    if ((socket_ = socket(PF_INET, mode_, IPPROTO_TCP)) == -1) {
        QString exception("error creating socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

Socket::~Socket() {
    shutdown(socket_, SHUT_RDWR);
    close(socket_);
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
    if (mode_ != kTCP) {
        return;
    }
    if (::listen(socket_, backlog) == -1) {
        QString exception("error listening on server socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

int Socket::receive(char* buffer, int length) const {
    int read = 0;

    if (mode_ == kTCP) {
        while ((read = recv(socket_, buffer, length, 0)) != -1) {
           buffer += read;
           length -= read;

           if (length == 0) {
               break;
           }
       }
    } else if (mode_ == kUDP) {
        read = recvfrom(socket_, buffer, length, 0, NULL, NULL);
    }


   return read;
}

int Socket::transmit(char *buffer, int length) const {
    if (mode_ == kTCP) {
        return send(socket_, buffer, length, 0);
    } else if (mode_ == kUDP) {
        return sendto(socket_, buffer, length, 0, (const sockaddr*) &peer_, sizeof(peer_));
    } else {
        return -1;
    }

}

Socket Socket::accept() {
    int newSocket;
    sockaddr_in info;
    socklen_t len = sizeof(info);
    newSocket = ::accept(socket_, (sockaddr*) &info, &len);
    return Socket(newSocket, mode_, info);
}
