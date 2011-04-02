#include <QDebug>
#include "serverstream.h"


class Helper: public QThread {
public:
	static void msleep(int ms)
	{
		QThread::msleep(ms);
	}
};

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

    Helper sleeper;
    while (!file->atEnd()) {
        memset((void*) (buffer + HEADER_LENGTH), 0, STREAMPACKETSIZE - HEADER_LENGTH);
        int length = file->read(buffer + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH) + HEADER_LENGTH;
        s.transmit(buffer, length);
        sleeper.msleep(85);
    }

    //delete[] pckt.data;

    file->close();
    emit signalTransferDone();
}
