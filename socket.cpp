#include "socket.h"

Socket::Socket(int type, int port, int packetSize) {
}

int Socket::SetAsServer() {
    return 1;
}

int Socket::TCPServer() {
    //emit packet recieved
   return 1;
}

int Socket::UDPServer() {
    //emit udp packet recieved
    return 1;
}

int Socket::SetupSocket(const char * str) {
    return 1;
}

int Socket::SetAsClient(const char * str) {
    return 1;
}

void Socket::createTCPSocket() {
}

void Socket::createUDPSocket() {
}

int Socket::tx(const char * str, int length) {
    return 1;
}

int Socket::tx(const QString str) {
    return tx(str.toLatin1().data(), str.length());
}

int Socket::tx(const char *str, int length, int socketDescriptor) {
    return 1;
}

int Socket::rx(char * str) {
    //TODO warren emit signal
    return 1;
}

void Socket::closeSocket() {
    //TODO warren socket closed signal
    //close(socketDescriptor_);
}
