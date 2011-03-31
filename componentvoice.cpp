#include "componentvoice.h"


ComponentVoice::ComponentVoice(const Socket& socket) : Component(kTransfer, socket) {
    //socket_ = new Socket(kUDP);
    //ap_ = new AudioPlayer(8000, 1, 8, 4096);
    //socket_->serverJoinMCast(inet_addr(MULTICASTIP), htons(MULTICASTPORT));
    //mic_ = new Microphone();
    //connect(mic_, SIGNAL(sendVoice(QByteArray*)),this, SLOT(transmitVoice(QByteArray*)));
    //micThread_ = new Thread();
    //micThread_->start();
    //mic_->moveToThread(micThread_);
}

ComponentVoice::~ComponentVoice() {
    delete socket_;
    //QThread::wait();
}

void ComponentVoice::transmitVoice(QByteArray * ba) {
    qDebug() << socket_->transmit(ba->data(), ba->length());
    delete ba;
}

void ComponentVoice::run() {
    mic_->startRecording();

}


