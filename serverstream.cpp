#include <QDebug>
#include "serverstream.h"

ServerStream::ServerStream(QString fileName) :cleanup_(false){
    fileName_ = fileName;
}

void ServerStream::slotStartTransfer(){
    file_ = new QFile(fileName_);
    buffer_ = new char[STREAMPACKETSIZE];

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
    }
    timer_ = new QTimer();
    connect(timer_,SIGNAL(timeout()),this,SLOT(slotTransmitOnTimer()));
    timer_->start(20); //will need some math on how fast to read
}

void ServerStream::slotTransmitOnTimer() {
    if (!file_->atEnd()) {
        memset((void*) (buffer_ + HEADER_LENGTH), 0, STREAMPACKETSIZE - HEADER_LENGTH);
        int length = file_->read(buffer_ + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        socket_->transmit(buffer_, length);
        return;
    }

    if (!cleanup_) {
        cleanup_ = true;
        timer_->stop();
        connect(this, SIGNAL(signalCleanup()), this, SLOT(slotCleanup()));
        emit signalCleanup();
    }
}

void ServerStream::slotCleanup() {
    delete timer_;
    delete socket_;
    delete[] buffer_;
    file_->close();
    delete file_;
    QThread::currentThread()->terminate();
}
