#include "server.h"

Server::Server(int port) : socket_(Socket(kTCP)) {

    socket_.bind(port);
    socket_.listen(5);
//    dispatcher_ = new Dispatcher();

//    connect(TCPServer_, SIGNAL(signalPacketRecieved(Message *)),
//            dispatcher_,SLOT(slotPacketRecieved(Message *)));
//    connect(TCPServer_, SIGNAL(signalSocketClosed(int)),
//            dispatcher_, SLOT(slotSocketClosed(int)));
//    connect(dispatcher_, SIGNAL(signalTxPckt(Message*)),
//            this, SLOT(slotTransmitMessage(Message*)));
//    connect(this, SIGNAL(signalServerStart()),
//            TCPServer_, SLOT(SetAsServer()));
}


void Server::slotTransmitMessage(Message * msg) {
//    switch(msg->type) {
//    case kUDP:
//        UDPServer_->tx(msg->payload, msg->idAddr);
//        break;
//    case kTCP:
//        TCPServer_->tx(msg->payload, msg->socketID);
//        break;
//    case kMulticast:
//        //not coded in socket yet
//        break;
//    }
}

