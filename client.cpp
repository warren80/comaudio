#include <QDebug>

#include "client.h"

Client::Client() : socket_(new Socket(kTCP)), running_(false) {

}

Client::~Client() {
    // prepare thread for close by aborting socket operations and stopping the loop from repeating.
    running_ = false;
    delete socket_;

    QThread::terminate();
    QThread::wait();
}

void Client::run() {
    running_ = true;
    while (running_) {
        bool deleteData = true;
        int msgSize;
        // receive the size of a packet and receive if successfull.
        switch (socket_->receive((char*) &msgSize, sizeof(int))) {
        case -1:
            // error
            break;
        case 0:
            // disconnect
            emit signalShutdown();
            running_ = false;
            return;
            break;
        default:
            // data
            char* buffer = new char[msgSize];
            socket_->receive(buffer, msgSize);

            int type;
            memcpy((void*) &type, buffer, sizeof(int));

            char* data;
            if ((unsigned)msgSize > sizeof(int)) {
                data = new char[msgSize - sizeof(int)];
                memcpy(data, buffer + sizeof(int), msgSize - sizeof(int));
            }

            switch (type) {
            case kTransfer:
                if (msgSize == sizeof(int)) {
                    emit signalFileFinished();
                    break;
                } else {
                    emit signalFileDataReceived(data, msgSize - sizeof(int));
                    deleteData = false;
                }

                break;
            case kStream:
                if (msgSize == sizeof(int)) {
                    emit signalSongName(QString());
                    emit signalStopStream();
                } else {
                    emit signalSongName(QString(buffer + sizeof(int)));
                }
                break;
            case kVoice:
                if(msgSize == sizeof(int) + 1) {
                    if (buffer[sizeof(int)] == 0) {
                        emit signalStopVoiceMessage();
                        return;
                    }
                }
                signalVoiceMessage(buffer,msgSize);
                deleteData = false;
                break;

            case kFileList:
                emit signalFileListReceived(data, msgSize - sizeof(int));
                deleteData = false;
                break;
            }

            if (deleteData && msgSize > (int) sizeof(int)) {
                if (data != 0) {
                    delete[] data;
                    data = 0;
                }
            }
            break;
        }
    }
}
