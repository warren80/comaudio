#include "dispatcher.h"


//TODO take all components out of thread and add them to a storage structure that can be itereated

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent) {
    compIterator_ = new ComponentIterator();
}

void Dispatcher::startComponent(Message * msg) {
    int id;
    if ((id = compIterator_->createComponent(msg)) == -1) {
        //failed connection stuff tx a close packet message
        qDebug("Dispatcher::createComponent() max component limit hit");
        return;
    }
}

void Dispatcher::slotPacketRecieved(Message * msg) {
    if (msg->payload->componentID > COMPONENTLIMIT) {
        return startComponent(msg);
    }
    if (compIterator_->clientMessage(msg) == -1) {
        qDebug("Dispatcher::slotPacketRecieved() component not found");
    }
}

void Dispatcher::slotPacketToTransmit(Message * msg) {
    emit signalTxPckt(msg);
}

void Dispatcher::slotSocketClosed(int socketID) {

}

