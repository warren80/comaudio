#include "componentvoice.h"
#include "audioplayer.h"
#include <QDebug>
#include <errno.h>

ComponentVoice::ComponentVoice(Socket* socket) :socket_(socket) {
    ap_ = 0;
    mic_ = 0;
    micThread_ = 0;
}

ComponentVoice::~ComponentVoice() {
    qDebug() <<  "Delete ComponentVoice";
    if (ap_ != 0) {
        delete ap_;
    }
    if (socket_ != 0) {
        delete socket_;
    }
    if (mic_ != 0) {
        delete mic_;
    }
    if (micThread_ != 0) {
        micThread_->terminate();
        micThread_->wait();
        delete micThread_;
    }
    qDebug() << "destruct voice";
}

void ComponentVoice::transmitVoice(QByteArray * ba) {
    Packet pckt;
    pckt.data = ba->data();
    pckt.length = ba->size();
    pckt.type = kVoice;
    socket_->transmit(pckt);
    delete ba;
    ba = 0;
}

void ComponentVoice::slotStartComponentVoice() {
    try {
        mic_ = new Microphone();
    } catch (const QString& e) {
        Packet pckt;
        pckt.data = new char;
        pckt.length = 1;
        pckt.type = kVoice;
        *pckt.data = 0;
        socket_->transmit(pckt);
        qDebug() << e;
        QString exception("Component Voice constructor failed: ");
        exception.append(strerror(errno));
        throw exception;
    }
    ap_ = new AudioPlayer(44100,2,16);

    micThread_ = new Thread();
    micThread_->start();
    mic_->moveToThread(micThread_);

    connect(mic_, SIGNAL(sendVoice(QByteArray*)),this, SLOT(transmitVoice(QByteArray*)));
    connect(this,SIGNAL(signalStartMic()),mic_,SLOT(startRecording()));

    emit signalStartMic();

}

void ComponentVoice::receiveData(char* data, int length) {
    ap_->appendBuffer(data + (2 * sizeof(int)), length - (2 * sizeof(int)));
    delete[] data;
    data = 0;
}

void ComponentVoice::slotStopVoiceComponent() {
    qDebug() << "Stopping voice component";
    mic_->stopRecording();
    ap_->pause();
    delete mic_;
    micThread_->terminate();
    micThread_->wait();
    mic_ = 0;
    delete ap_;
    ap_ = 0;
    disconnect();
    QThread::currentThread()->exit();
}


