#include "serverfiletransfer.h"
#include "packet.h"
#include <QDebug>

ServerFileTransfer::ServerFileTransfer(QString filename, Socket *s)
    :fileName_(filename), socket_(s) {
}

ServerFileTransfer::~ServerFileTransfer() {
    qDebug() <<  "Delete ServerFileTransfer";
}

void ServerFileTransfer::slotStartTransfer(){
    fileTransferInProgress.lock();
    QFile *file = new QFile("./Songs/" + fileName_);
    Packet pckt;

    if(!file->open(QIODevice::ReadOnly)) {
        cleanup();
        return;
    }

    pckt.data = new char[FT_DATA_SIZE];

    while (!file->atEnd()) {
        pckt.length = file->read(pckt.data,FT_DATA_SIZE);
        pckt.type = kTransfer;
        socket_->transmit(pckt);
    }
    if (pckt.data != 0) {
        delete[] pckt.data;
        pckt.data = 0;
    }
    file->close();
    cleanup();
}

void ServerFileTransfer::cleanup() {
    Packet pckt;
    if (pckt.data != 0) {
        delete[] pckt.data;
        pckt.data = 0;
    }
    pckt.length = 0;
    pckt.type = kTransfer;
    socket_->transmit(pckt);
    fileTransferInProgress.unlock();
    QThread::currentThread()->terminate();
}
