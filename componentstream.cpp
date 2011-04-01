//#include <QDebug>

#include "componentstream.h"

ComponentStream::ComponentStream() : Component(new Socket(kUDP)), audioPlayer_(NULL) {
    socket_->clientJoinMCast(inet_addr(MULTICAST_IP), MULTICAST_PORT);
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
            // process buffer to buffer + HEADER_LENGTH - 1

            audioPlayer_->appendBuffer(buffer + HEADER_LENGTH, msgSize - HEADER_LENGTH);

            // remove the memory since its now in the audio player
            delete[] buffer;
            break;
        }
    }
}
