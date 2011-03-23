#include "dispatcher.h"


//TODO take all components out of thread and add them to a storage structure that can be itereated

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent)
{
    pSocket = new Socket(0,0);
    connect(pSocket,SIGNAL(signalPacketRecieved(Packet *)),this,SLOT(slotPacketRecieved(Packet *)));
    connect(pSocket, SIGNAL(signalSocketClosed(int)), this, SLOT(slotSocketClosed(int)));
    void signalPacketRecieved(Packet p);
    void signalSocketClosed(int socketID);
}

void Dispatcher::slotPacketRecieved(Packet * pckt) {
    /*
    Packet *p = (Packet *) pckt;
    if (p->packetType & AUDIOCOMPONENT) {
        emit signalPacketToComponentAudio(pckt);
    }
    if (p->packetType & VOICECOMPONENT) {
        emit signalPacketToComponentVoice(pckt);
    }
    if (p->packetType & TEXTCOMPONENT) {
        emit signalPacketToComponentText(pckt);
    }
    if (p->packetType & FILECOMPONENT) {
        emit signalPacketToComponentFile(pckt);
    }
    */
}

void Dispatcher::startComponent(int type, int socketID) {
    Component * pComponent;
    switch (type) {
    case AUDIOCOMPONENT:
        pComponent = new ComponentAudio(socketID);
        break;
    case FILECOMPONENT:
        pComponent = new ComponentFile(socketID);
        break;
    case TEXTCOMPONENT:
        pComponent = new ComponentText(socketID);
        break;
    case VOICECOMPONENT:
        pComponent = new ComponentVoice(socketID);
        break;
    }
}

void Dispatcher::slotPacketToTransmit(Message *) {
    //todo code to transmit all transmits must be done in same thread
    //so this may have to emit something. to be in right thread
}

void Dispatcher::slotSocketClosed(int socketID) {

}

