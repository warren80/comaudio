#include "componentvoice.h"


ComponentVoice::ComponentVoice(const Socket& socket) : Component(kTransfer, socket) {

    mic_ = new Microphone();
    connect(mic_, SIGNAL(sendVoice(QByteArray*)),this, SLOT(transmitVoice(QByteArray*)));
    micThread_ = new Thread();
    micThread_->start();
    mic_->moveToThread(micThread_);
}

ComponentVoice::~ComponentVoice() {
    delete socket_;
    //QThread::wait();
}

void ComponentVoice::transmitVoice(QByteArray * ba) {
    //need to emit a const char * and length to what ever thread is handling transmits
    delete ba;
}

void ComponentVoice::receiveData(char* data, int length) {

}

void ComponentVoice::run() {
    mic_->startRecording();

}


