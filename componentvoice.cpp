#include "componentvoice.h"
#include "audioplayer.h"
#include <QDebug>

ComponentVoice::ComponentVoice(Socket* socket) : Component(socket) {
    qDebug() << socket;
    socket_ = socket;
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
    Packet pckt;
    pckt.data = ba->data();
    pckt.length = ba->size();
    pckt.type = kVoice;
    socket_->transmit(pckt);
    delete ba;
}

void ComponentVoice::receiveData(char* data, int length) {
    ap_->appendBuffer(data + (2 * sizeof(int)), length - (2 * sizeof(int)));
}

void ComponentVoice::run() {
    mic_->startRecording();

}

void ComponentVoice::slotStopVoiceComponent() {
    mic_->stopRecording();
    ap_->pause();
    micThread_->exit();
    delete mic_;
    delete ap_;
    disconnect();
    QThread::currentThread()->exit();
}


