#include "dispatcher.h"


//TODO take all components out of thread and add them to a storage structure that can be itereated

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent)
{
    pSocket = new SocketClass(0,0);
}

void Dispatcher::slotPacketRecieved(void * pckt) {
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

}

void Dispatcher::startComponent(int type) {
    Component * pComponent;
    Thread * t = new Thread();
    t->start();
    switch (type) {
    case AUDIOCOMPONENT:
        pComponent = new ComponentAudio();
        connect(this,SIGNAL(signalStartComponentAudio()),pComponent, SLOT(slotStart()));
        connect(this,SIGNAL(signalPacketToComponentAudio()),pComponent, SLOT(slotPacketReceived(void*)));
        connect(pComponent, SIGNAL(signalTxPckt(void*)), this, SLOT(slotPacketToTransmit(void*)));
        pComponent->moveToThread(t);
        emit signalStartComponentAudio();
        break;
    case FILECOMPONENT:
        pComponent = new ComponentFile();
        connect(this,SIGNAL(signalStartComponentFile()),pComponent, SLOT(slotStart()));
        connect(this,SIGNAL(signalPacketToComponentFile()),pComponent, SLOT(slotPacketReceived(void*)));
        connect(pComponent, SIGNAL(signalTxPckt(void*)), this, SLOT(slotPacketToTransmit(void*)));
        pComponent->moveToThread(t);
        emit signalStartComponentFile();
        break;
    case TEXTCOMPONENT:
        pComponent = new ComponentText();
        connect(this,SIGNAL(signalStartComponentText()),pComponent, SLOT(slotStart()));
        connect(this,SIGNAL(signalPacketToComponenText()),pComponent, SLOT(slotPacketReceived(void*)));
        connect(pComponent, SIGNAL(signalTxPckt(void*)), this, SLOT(slotPacketToTransmit(void*)));
        pComponent->moveToThread(t);
        emit signalStartComponentText();
        break;
    case VOICECOMPONENT:
        pComponent = new ComponentVoice();
        connect(this,SIGNAL(signalStartComponentVoice()),pComponent, SLOT(slotStart()));
        connect(this,SIGNAL(signalPacketToComponentVoice()),pComponent, SLOT(slotPacketReceived(void*)));
        connect(pComponent, SIGNAL(signalTxPckt(void*)), this, SLOT(slotPacketToTransmit(void*)));
        pComponent->moveToThread(t);
        emit signalStartComponentVoice();
        break;
    }
}

void Dispatcher::slotPacketToTransmit(void*) {
    //todo code to transmit all transmits must be done in same thread
    //so this may have to emit something. to be in right thread
}

