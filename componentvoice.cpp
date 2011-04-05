#include "componentvoice.h"
#include "audioplayer.h"
#include <QDebug>
#include <errno.h>

ComponentVoice::ComponentVoice(Socket* socket) :socket_(socket) {
    qDebug () << "Socket" << socket << " Socket_ " << socket;
    //socket_ = socket;
    try {
        mic_ = new Microphone();
    } catch (const QString& e) {
        Packet pckt;
        pckt.length = 0;
        pckt.type = kVoice;
        socket->transmit(pckt);
        qDebug() << e;
        QString exception("Component Voice constructor failed: ");
        exception.append(strerror(errno));
        return;
    }


    micThread_ = new Thread();
    mic_->moveToThread(micThread_);
    micThread_->start();
    connect(mic_, SIGNAL(sendVoice(QByteArray*)),this, SLOT(transmitVoice(QByteArray*)));
    mic_->startRecording();
    ap_ = new AudioPlayer(44100,2,16);
}

ComponentVoice::~ComponentVoice() {
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
    delete[] data;
}

void ComponentVoice::slotStopVoiceComponent() {
    qDebug() << "Stopping voice component";
    mic_->stopRecording();
    ap_->pause();
    micThread_->exit();
    delete mic_;
    delete ap_;
    disconnect();
    QThread::currentThread()->exit();
}


