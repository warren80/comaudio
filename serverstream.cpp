#include "serverstream.h"

ServerStream::ServerStream() :cleanup_(false),timer_(0), file_(0), socket_(0){
    file_ = new QFile();
    buffer_ = new char[STREAMPACKETSIZE];
}

ServerStream::~ServerStream() {
    if (file_ != 0) {
        delete file_;
    }
    if (timer_ != 0) {
        delete timer_;
    }
    if (buffer_ != 0) {
        delete[] buffer_;
    }
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
        emit signalPrintF(QString("failedto join MultiCast"));
    }
    if(!file_->open(QIODevice::ReadOnly)) {
        emit signalTransferDone();
        return;
    }

    if(file_->size() < HEADER_LENGTH) {
        emit signalPrintF(QString("Invalid wav format"));
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
        int length = file_->read(buffer_ + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        socket_->transmit(buffer_, length);
        return;
    } else {
        timer_->stop();
    }

}

void ServerStream::slotCleanup() {
    if (timer_ != NULL) {
        timer_->stop();
    }
}
