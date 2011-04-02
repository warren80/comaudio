#include <QDebug>
#include "serverstream.h"

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
    char* buffer = new char[STREAMPACKETSIZE];

    Socket s(kUDP);
    if (!s.serverJoinMCast(inet_addr(MULTICAST_IP), htons(MULTICAST_PORT))) {
        qDebug() << "failed to join MultiCast";
    }


    if(!file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        emit signalTransferDone();
        return;
    }

    qDebug() << "file size:" << file->size();

    if(file->size() < HEADER_LENGTH) {
        qDebug() << "invalid wave file format";
        emit signalTransferDone();
        return;
    }

    //pckt.data = new char[STREAMDATALENGTH];

    if (file->read(buffer,HEADER_LENGTH) != HEADER_LENGTH) {
        emit signalTransferDone();
        return;
    }

    int num = 0;
    while (!file->atEnd()) {
        memset((void*) (buffer + HEADER_LENGTH), 0, STREAMPACKETSIZE - HEADER_LENGTH);
        int length = file->read(buffer + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        s.transmit(buffer, length);
        num++;
    }

    qDebug() << num;
    //delete[] pckt.data;

    file->close();
    emit signalTransferDone();
}
