#include <QDebug>
#include "serverstream.h"

ServerStream::ServerStream() :cleanup_(false){
    file_ = new QFile();
    buffer_ = new char[STREAMPACKETSIZE];

}

void ServerStream::slotStartTransfer(QString filename){
    int byterate;
    double numerator;
    double divisor;
    if (file_->isOpen()) {
        file_->close();
    }
    timer_ = new QTimer();

    file_->setFileName(filename);

    socket_ = new Socket(kUDP);
    if (!socket_->serverJoinMCast(inet_addr(MULTICAST_IP), htons(MULTICAST_PORT))) {
        qDebug() << "failed to join MultiCast";
    }

    if(!file_->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        emit signalTransferDone();
        return;
    }

    if(file_->size() < HEADER_LENGTH) {
        qDebug() << "invalid wave file_ format";
        emit signalTransferDone();
        return;
    }

    //pckt.data = new char[STREAMDATALENGTH];

    if (file_->read(buffer_,HEADER_LENGTH) != HEADER_LENGTH) {
        emit signalTransferDone();
        return;
    }

    connect(timer_,SIGNAL(timeout()),this,SLOT(slotTransmitOnTimer()));

    divisor = STREAMPACKETSIZE - HEADER_LENGTH;
    qDebug() << "divisor" << divisor;
    memcpy(&byterate,buffer_ + BYTERATEOFFSET, BYTERATESIZE);
    qDebug() << "byterate" << byterate;
    numerator = byterate;
    qDebug() << "num" << numerator;
    divisor = numerator /  divisor;

    qDebug() << "tics per second" << divisor;
    divisor = 1 / divisor;
    qDebug() << "1 / tics per second";// << divisor;
    qDebug() << divisor;
    //qDebug() << byterate;
    divisor *= 1000;
    qDebug() << divisor;
    byterate = divisor - 2;
    qDebug() << byterate;
    timer_->start(byterate);
    WaveHeader * wh = AudioPlayer::parseWaveHeader(buffer_);
    qDebug() << "bits per sample " << wh->bitsPerSample << " Channels " << wh->channels << " frequency " << wh->frequency;
 //   int i = (1000 /(byterate / (STREAMPACKETSIZE - HEADER_LENGTH)) - 3);
   // qDebug() << i;
}

void ServerStream::slotTransmitOnTimer() {
    if (!file_->atEnd()) {
        memset((void*) (buffer_ + HEADER_LENGTH), 0, STREAMPACKETSIZE - HEADER_LENGTH);
        int length = file_->read(buffer_ + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        socket_->transmit(buffer_, length);
        return;
    }
    timer_->stop();
}



void ServerStream::slotCleanup() {
   timer_->stop();

}
