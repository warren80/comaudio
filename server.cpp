
#include "server.h"
#include "thread.h"

Server::Server(int port) {
    Thread * UDPThread = new Thread;
    Thread * TCPThread = new Thread;
    dispatcher_ = new Dispatcher();
    //do some connects
    UDPServer_ = new Socket(UDP, port);
    connect(UDPServer_,SIGNAL(signalPacketRecieved(Packet *)),
            dispatcher_,SLOT(slotPacketRecieved(Packet *)));
    //connect transmitter
    UDPServer_->moveToThread(UDPThread);
    UDPThread->start();
    UDPServer_->SetAsServer();


    TCPServer_ = new Socket(TCP, port);
    connect(TCPServer_, SIGNAL(signalPacketRecieved(Packet *)),
            dispatcher_,SLOT(slotPacketRecieved(Packet *)));
    connect(TCPServer_, SIGNAL(signalSocketClosed(int)),
            dispatcher_, SLOT(slotSocketClosed(int)));
    //connect transmitter

    TCPServer_->moveToThread(TCPThread);
    TCPThread->start();
    TCPServer_->SetAsServer();


    //connect(pSocket, SIGNAL(signalSocketClosed(int)), dispatcher_, SLOT(slotSocketClosed(int)));

}


void Server::slotTransmitMessage(Message * msg) {
    switch(msg->type) {
    case kUDP:
        UDPServer_->tx(msg->payload);
        break;
    case kTCP:
        TCPServer_->tx(msg->payload);
        break;
    case kMulticast:
        //not coded in socket yet
        break;
    }
}

