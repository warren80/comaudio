#include "componentvoice.h"
#include "audioplayer.h"

ComponentVoice::ComponentVoice(Socket* socket) : Component(socket) {

    mic_ = new Microphone();
    connect(mic_, SIGNAL(sendVoice(QByteArray*)),this, SLOT(transmitVoice(QByteArray*)));
    micThread_ = new Thread();
    micThread_->start();
    mic_->moveToThread(micThread_);
    mic_->startRecording();

    ap_ = new AudioPlayer(44100,1,16);

}

ComponentVoice::~ComponentVoice() {
    delete socket_;
    //QThread::wait();
}

void ComponentVoice::transmitVoice(QByteArray * ba) {
    ap_->appendBuffer(ba->data(),ba->size());
    qDebug() << ("Bytes sent to audioPlayer %d", ba->size());
    delete ba;
}

void ComponentVoice::receiveData(char* data, int length) {

}

void ComponentVoice::run() {
    mic_->startRecording();

}


