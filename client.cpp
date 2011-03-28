#include <QDebug>

#include "client.h"

Client::Client() : socket_(new Socket(kTCP)), running_(false) {
    qDebug() << "Client starting...";
}

Client::~Client() {
    // prepare thread for close by aborting socket operations and stopping the loop from repeating.
    running_ = false;
    delete socket_;

    // wait for the thread to finish.
    QThread::wait();
}

void Client::run() {

    running_ = true;
    while (running_) {
        int msgSize;
        // receive the size of a packet and receive if successfull.
        switch (socket_->receive((char*) &msgSize, sizeof(int))) {
        case -1:
            // error
            break;
        case 0:
            // disconnect
            break;
        default:
            // data
            char* buffer = new char[msgSize];
            socket_->receive(buffer, msgSize);

            // temp. echo received data
            qDebug() << buffer;
            delete[] buffer;

            break;
        }
    }
}
