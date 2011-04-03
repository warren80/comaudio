#include <QDebug>
#include "serverstream.h"

ServerStream::ServerStream() :cleanup_(false){
    file_ = new QFile();
    buffer_ = new char[STREAMPACKETSIZE];
}

void ServerStream::slotStartTransfer(QString filename){
    int byterate;
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

    memcpy(&byterate,buffer_ + BYTERATEOFFSET, BYTERATESIZE);
    byterate = ((1  / ((double) byterate /
                       ((double) STREAMPACKETSIZE -
                        (double) HEADER_LENGTH))) * 1000) - 2;
    timer_->start(byterate);
}

void ServerStream::slotTransmitOnTimer() {
    if (!file_->atEnd()) {
        memset((void*) (buffer_ + HEADER_LENGTH), 0, STREAMPACKETSIZE - HEADER_LENGTH);
        int length = file_->read(buffer_ + HEADER_LENGTH,
                                 STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        socket_->transmit(buffer_, length);
        return;
    }
    timer_->stop();
}



void ServerStream::slotCleanup() {
   timer_->stop();

}
