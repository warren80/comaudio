#include "server.h"
#include "thread.h"

Server::Server(int port) {
    Thread * UDPThread = new Thread;
    Thread * TCPThread = new Thread;
    dispatcher_ = new Dispatcher();
    //do some connects
    UDPServer_ = new SocketClass(UDP, port);
    //do some connects
    UDPServer_->moveToThread(UDPThread);
    UDPThread->start();
    UDPServer_->SetAsServer();

    TCPServer_ = new SocketClass(TCP, port);
    //do some connects
    TCPServer_->moveToThread(TCPThread);
    TCPThread->start();
    TCPServer_->SetAsServer();


}

void Server::slotMessageFromSocket(void * buf, int length) {
    emit signalDispatcher(buf, length);
}

void Server::txMessage(void * buf, int length, int socketType, int socketDescriptor) {
    if (socketType == UDP) {
        UDPServer_->tx((MessageStruct *)buf, length);
        return;
    }
    TCPServer_->tx((MessageStruct *)buf, length, socketDescriptor);
}

