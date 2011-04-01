#include <QDebug>
#include "serverstream.h"
#include "packet.h"

/*ServerStream::ServerStream(QString fileNamePath) {
    Thread *thread = new Thread();

    ServerStream *sfwo = new ServerStream(fileNamePath);
    connect(this, SIGNAL(signalStreamFile()), sfwo, SLOT(startTransfer()));
    connect(sfwo, SIGNAL(signalTransferDone()), thread, SLOT(deleteLater()));
    thread->start();
    emit signalStreamFile();
}
*/

ServerStream::ServerStream(QString fileNamePath)
    :fileNamePath_(fileNamePath) {
}

void ServerStream::slotStartTransfer(){
    QFile *file = new QFile(fileNamePath_);
    Packet pckt;

    Socket s(kUDP);
    s.serverJoinMCast(inet_addr(MULTICAST_IP), htons(MULTICAST_PORT));

    if(file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        emit signalTransferDone();
        return;
    }

    qDebug() << file->size();

    if(file->size() < HEADER_LENGTH) {
        qDebug() << "invalid wave file format";
        emit signalTransferDone();
        return;
    }

    if (file->read(pckt.data,HEADER_LENGTH) != HEADER_LENGTH) {
        emit signalTransferDone();
    }

    while (!file->atEnd()) {
        pckt.data = new char[STREAMDATALENGTH];
        pckt.length = file->read(pckt.data + HEADER_LENGTH,STREAMDATALENGTH);
        pckt.type = kStream;
        s.transmit(pckt);
        delete[] pckt.data;
    }

    file->close();
    emit signalTransferDone();
}
