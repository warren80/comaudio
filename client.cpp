#include <QDebug>

#include "client.h"

Client::Client() : socket_(Socket(kTCP)), running_(false) {
    qDebug() << "Client starting...";
}

void Client::run() {

    while (running_) {
        int msgSize;
        switch (socket_.receive((char*) &msgSize, sizeof(int))) {
        case -1:
            // error
            break;
        case 0:
            // disconnect
            break;
        default:
            // data
            char* buffer = new char[msgSize];
            socket_.receive(buffer, msgSize);

            // temp. echo received data
            qDebug() << buffer;
            delete[] buffer;

            break;
        }
    }
}
