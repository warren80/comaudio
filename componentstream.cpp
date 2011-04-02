//#include <QDebug>

#include "componentstream.h"
#include "componenttype.h"

ComponentStream::ComponentStream() : Component(new Socket(kUDP)), audioPlayer_(NULL) {
    if (!socket_->clientJoinMCast(inet_addr(MULTICAST_IP), htons(MULTICAST_PORT))) {
        qDebug() << "error joining multicast";
    }
}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (audioPlayer_ != NULL) {
        delete audioPlayer_;
    }
    audioPlayer_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

//void ComponentStream::receiveData(char *data, int length) {
//
//    short l;
//    if (audioPlayer_ != NULL) {
//        //parse the wave header packet
//        //setupAudio()
//    }
//    memcpy(&l, data + HEADERLENGTH, sizeof(short));
//    audioPlayer_->appendBuffer(data + HEADERLENGTH + 2,l);
//}

void ComponentStream::run() {
    running_ = true;
    while (running_) {
        int msgSize;
        // receive the size of a packet and receive if successfull.
        switch (socket_->receive((char*) &msgSize, sizeof(int))) {
        case -1:
            qDebug() << "error";
            // error
            break;
        case 0:
            qDebug() << "disconnect";
            // disconnect
            break;
        default:
            qDebug() << "data";
            // data
            char* buffer = new char[msgSize];
            socket_->receive(buffer, msgSize);

            // check if the wave header stuff is the same
            // process buffer to buffer + HEADER_LENGTH - 1
            if (audioPlayer_ == NULL) {
                setupAudio(44100, 2, 16, 100000);
            }

            audioPlayer_->appendBuffer(buffer + sizeof(ComponentType) + HEADER_LENGTH, msgSize - HEADER_LENGTH);

            // remove the memory since its now in the audio player
            delete[] buffer;
            break;
        }
    }
}
