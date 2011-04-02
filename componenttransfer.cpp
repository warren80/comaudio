#include "componenttransfer.h"
#include <QFile>
#include <QList>
#include <QMessageBox>

#include "serverfiletransfer.h"

#include "componentstream.h"
#include "componenttype.h"

ComponentTransfer::ComponentTransfer(Socket * s, QString fileName) : Component(s),
fileName_(fileName) {
}

void ComponentTransfer::run() {
    running_ = true;
    int read;
    char * buffer;
    QFile file(QDir::currentPath().append("/Songs/").append(fileName_));
    //buffer

    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical,
                                 "Error", "Cannot open file or file doesn't exist",
                                 QMessageBox::Ok).exec();
        return;
    }

    buffer = new char[FT_PACKET_SIZE];
    while (running_) {
        int msgSize;
        // receive the size of a packet and receive if successfull.
        switch (socket_->receive((char*) &msgSize, sizeof(int))) {
        case -1:
            qDebug() << "error";
            // error
            break;
        case 0:

            break;
        default:
            read = socket_->receive(buffer, msgSize);
            file.write(buffer + FT_HEADER_SIZE, read - FT_HEADER_SIZE);
            break;
        }
    }
    file.close();
}
