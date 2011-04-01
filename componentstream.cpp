#include <QDebug>

#include "componentstream.h"

ComponentStream::ComponentStream(const Socket& socket) : Component(socket), audioPlayer_(NULL) {

}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (audioPlayer_ != NULL) {
        delete audioPlayer_;
    }
    audioPlayer_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

void ComponentStream::receiveData(char *data, int length) {
    short l;
    if (audioPlayer_ != NULL) {
        //parse the wave header packet
        //setupAudio()
    }
    memcpy(&l, data + HEADERLENGTH, sizeof(short));
    audioPlayer_->appendBuffer(data + HEADERLENGTH + 2,l);
}

void ComponentStream::transmitData(char *data, int length) {
    socket_.transmit(data, length);
}

void ComponentStream::streamFileTransfer(QString fileNamePath) {
    Thread *thread = new Thread();

    StreamFileWorkerObject *sfwo = new StreamFileWorkerObject(fileNamePath);
    connect(this, SIGNAL(signalStreamFile()), sfwo, SLOT(startTransfer()));
    connect(sfwo, SIGNAL(signalTransferDone()), thread, SLOT(deleteLater()));
    thread->start();
    emit signalStreamFile();
}

StreamFileWorkerObject::StreamFileWorkerObject(QString fileNamePath)
    :fileNamePath_(fileNamePath) {
}

void StreamFileWorkerObject::slotStartTransfer(){
    QFile *file = new QFile(fileNamePath_);
    char* streamPacket;
    QByteArray *data;
    short *length = new short[0];

    Socket *s = new Socket(kUDP);
    s->serverJoinMCast(inet_addr(MULTICASTIP), htons(MULTICASTPORT));

    if(file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        emit signalTransferDone();
        return;
    }

    if(file->size() < HEADERLENGTH) {
        qDebug() << "invalid wave file format";
        emit signalTransferDone();
        return;
    }

    streamPacket = new char[STREAMPACKETSIZE];
    memcpy(streamPacket,file->read(HEADERLENGTH).data(), HEADERLENGTH);

    while((data = new QByteArray(file->read(STREAMDATALENGTH).data()))->length() > 0) {
        *length = (short) data->length();
        memcpy(streamPacket + HEADERLENGTH, length, sizeof(short));
        memcpy(streamPacket + HEADERLENGTH + 2, data->data(),*length);
        s->transmit(streamPacket, STREAMDATALENGTH);
    }
    delete(streamPacket);
    delete(data);
    file->close();
    emit signalTransferDone();
}
