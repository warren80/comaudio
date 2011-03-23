#include "dispatcher.h"


//TODO take all components out of thread and add them to a storage structure that can be itereated

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent) {
    compIterator_ = new ComponentIterator();
}

void Dispatcher::startComponent(Message * msg) {
    int id;
    Component * pComponent;
    if ((id = compIterator_->reserveID()) == -1) {
        //failed connection stuff tx a close packet message
        qDebug("Dispatcher::createComponent() max component limit hit");
        return;
    }

    switch (msg->payload->componentID) {
    case AUDIOCOMPONENT:
        pComponent = new ComponentAudio(msg->socketID);
        break;
    case FILECOMPONENT:
        pComponent = new ComponentFile(msg->socketID);
        break;
    case TEXTCOMPONENT:
        pComponent = new ComponentText(msg->socketID);
        break;
    case VOICECOMPONENT:
        pComponent = new ComponentVoice(msg->socketID);
        break;
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

