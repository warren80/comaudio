#include <QDebug>

#include "componentstream.h"

ComponentStream::ComponentStream(const Socket& socket) : Component(socket), audioPlayer_(NULL) {

}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (audioPlayer_ != NULL) {
        delete audioPlayer_;
    }
    audioPlayer_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

void ComponentStream::receiveData(char *data, int length) {
    audioPlayer_->appendBuffer(data, length);
}

void ComponentStream::transmitData(char *data, int length) {
    socket_.transmit(data, length);
}


void ComponentStream::run() {
    running_ = true;

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

            // check if the wave header stuff is the same

            receiveData(data, msgSize);

            // remove the memory since its now in the audio player
            delete[] buffer;
            break;
        }
    }
}
