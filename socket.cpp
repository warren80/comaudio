#include <QDebug>
#include <QString>
#include <errno.h>

#ifndef _WIN32
#include <sys/socket.h>
#define CLOSESOCKET(x) close(x)
#endif

#ifdef _WIN32
#define SHUT_RDWR 2
#define CLOSESOCKET(x) closesocket(x)
#endif

#include "socket.h"

Socket::Socket(NetMode mode) : mode_(mode) {
    multicast_ = false;

    if ((socket_ = socket(PF_INET, mode_, 0)) == -1) {
        QString exception("error creating socket: ");
        exception.append(strerror(errno));
        throw exception;
    }

    int arg = 1;

    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*) &arg, sizeof(arg)) == -1) {
        QString exception("error setting up socket: ");
        exception.append(strerror(errno));
        throw exception;
    }
}

Socket::~Socket() {
    if (multicast_) {
        ip_mreq stMreq;
        stMreq.imr_multiaddr.s_addr = inet_addr("234.5.6.7");
        stMreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(socket_, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == -1) {
            //TODO
        }
        multicast_ = false;
    }

    shutdown(socket_, SHUT_RDWR);
    CLOSESOCKET(socket_);
}

void Socket::bind(int port, in_addr_t listenTo) {
    sockaddr_in bindTo;
    memset((char*) &bindTo, 0, sizeof(bindTo));
    bindTo.sin_family = AF_INET;
    bindTo.sin_port = port;
    bindTo.sin_addr.s_addr = listenTo;

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

int Socket::receive(char* buffer, int length) {
    int read = 0;

    if (mode_ == kTCP) {
        while ((read = recv(socket_, buffer, length, 0)) != -1) {
           buffer += read;
           length -= read;

           if (read == 0) {
               return 0;
           }

           if (length == 0) {
               break;
           }
       }
    } else if (mode_ == kUDP) {
        read = recvfrom(socket_, buffer, length, 0, NULL, NULL);
    }

    return read;
}

int Socket::transmit(const char *buffer, int length) {
    int sent = -1;

    if (mode_ == kTCP) {
        sent = send(socket_, buffer, length, 0);
    } else if (mode_ == kUDP) {
        sent = sendto(socket_, buffer, length, 0, (const sockaddr*) &peer_, sizeof(peer_));
    }
    return sent;
}


int Socket::transmit(const Packet& packet) {
    int totalSize = packet.length + sizeof(packet.length) + sizeof(packet.type);
    char* data = new char[totalSize];
    int length = packet.length + sizeof(int);
    memcpy((void*) data, (void*) &length, sizeof(packet.length));
    int type = packet.type;
    memcpy((void*) (data + sizeof(packet.length)), (void*) &type, sizeof(int));
    if (packet.length != 0) {
        memcpy((void*) (data + sizeof(packet.length) + sizeof(int)), (void*) packet.data, packet.length);
    }
    return transmit(data, totalSize);
}

Socket* Socket::accept() const {
    int newSocket;
    sockaddr_in info;
    socklen_t len = sizeof(info);
    newSocket = ::accept(socket_, (sockaddr*) &info, &len);
    return new Socket(newSocket, mode_, info);
}

bool Socket::connect(in_addr_t address, uint16_t port) const {
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = address;
    serverAddress.sin_port = port;

    if (::connect(socket_, (const sockaddr*) &serverAddress, sizeof(serverAddress)) != -1) {
        return true;
    } else {
        return false;
    }
}

bool Socket::clientJoinMCast(in_addr_t address, uint16_t port) {
    ip_mreq stMreq;

    try {
        this->bind(port, htonl(INADDR_ANY));
    } catch (...) {
        return false;
    }

    stMreq.imr_multiaddr.s_addr = address;
    stMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    //join mcast session
    if (setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == -1) {
        return false;
    }

    multicast_ = true;

    return true;
}

bool Socket::serverJoinMCast(in_addr_t address, uint16_t port) {
    bool    f  = false;
    u_long  lTTL = 1;
    ip_mreq serverAddress;

    peer_.sin_family =      AF_INET;
    peer_.sin_addr.s_addr = address;
    peer_.sin_port =        port;

    try {
        this->bind(port, htonl(INADDR_ANY));
    } catch (...) {
        return false;
    }

    serverAddress.imr_multiaddr.s_addr = address;
    serverAddress.imr_interface.s_addr = INADDR_ANY;
    //join multicast address
    if (setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&serverAddress, sizeof(serverAddress)) == -1) {
        return false;
    }
    //set time to live to 1
    if (setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&lTTL, sizeof(lTTL)) == -1) {
        return false;
    }
    //disable loop back
    if (setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&f, sizeof(f)) == -1) {
        return false;
    }

    multicast_ = true;

    return true;
}
