
#include "server.h"
#include "thread.h"

Server::Server(int port) {
    Thread * UDPThread = new Thread;
    Thread * TCPThread = new Thread;
    UDPThread->start();
    TCPThread->start();
    dispatcher_ = new Dispatcher();
    //do some connects
    UDPServer_ = new Socket(UDP, port);
    TCPServer_ = new Socket(TCP, port);

    connect(UDPServer_,SIGNAL(signalPacketRecieved(Message *)),
            dispatcher_,SLOT(slotPacketRecieved(Message *)));
    connect(TCPServer_, SIGNAL(signalPacketRecieved(Message *)),
            dispatcher_,SLOT(slotPacketRecieved(Message *)));
    connect(TCPServer_, SIGNAL(signalSocketClosed(int)),
            dispatcher_, SLOT(slotSocketClosed(int)));
    connect(dispatcher_, SIGNAL(signalTxPckt(Message*)),
            this, SLOT(slotTransmitMessage(Message*)));
    connect(this, SIGNAL(signalServerStart()),
            UDPServer_, SLOT(SetAsServer()));
    connect(this, SIGNAL(signalServerStart()),
            TCPServer_, SLOT(SetAsServer()));

    UDPServer_->moveToThread(UDPThread);
    TCPServer_->moveToThread(TCPThread);
    emit signalServerStart();
}


void Server::slotTransmitMessage(Message * msg) {
    switch(msg->type) {
    case kUDP:
        UDPServer_->tx(msg->payload, msg->idAddr);
        break;
    case kTCP:
        TCPServer_->tx(msg->payload, msg->socketID);
        break;
    case kMulticast:
        //not coded in socket yet
        break;
    }
}

