#include "serverfiletransfer.h"

#include <QDebug>
#include <QMessageBox>
#include "serverfiletransfer.h"
#include "packet.h"

serverFileTransfer::serverFileTransfer(QString filename, Socket *s)
    :fileName_(filename), socket_(s) {
}

void serverFileTransfer::slotStartTransfer(){
    QFile *file = new QFile(fileName_);
    Packet pckt;

    if(!file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        emit signalTransferDone();
        return;
    }

    pckt.data = new char[FT_DATA_SIZE];

    while (!file->atEnd()) {
        pckt.length = file->read(pckt.data,FT_DATA_SIZE);
        pckt.type = kTransfer;
        socket_->transmit(pckt);
    }

    delete[] pckt.data;

    file->close();
    emit signalTransferDone();
}