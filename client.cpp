#include <QDebug>

#include "client.h"

Client::Client(char *ipAddr, int port, QString alias) : alias_(alias), socket_(Socket(kTCP)), running_(false) {
    qDebug() << "Client starting...";
    in_addr_t ip;

    if((ip = inet_addr(ipAddr)) == INADDR_NONE) {
        qDebug() << "Invalid ip";
        return;
    }

    if(socket_.connect(ip, htons(port))) {
        qDebug() << "client: connect() error: " + QString::number(errno);
        return;
    }
}

void Client::run() {
    running_ = true;

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
