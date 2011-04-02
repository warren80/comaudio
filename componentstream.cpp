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

void ComponentStream::run() {
    running_ = true;
    while (running_) {

        char* buffer = new char[STREAMPACKETSIZE];

        switch (socket_->receive(buffer, STREAMPACKETSIZE)) {
        case -1:
            // error
            break;
        case 0:
            // disconnect
            break;
        default:
            if (audioPlayer_ == NULL) {
                setupAudio(44100, 2, 16, 358000000);
            }

            emit signalReceivedData(STREAMPACKETSIZE);

            audioPlayer_->appendBuffer(buffer + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH);

        }

        delete[] buffer;
    }

//        int msgSize;
//        // receive the size of a packet and receive if successfull.
//        switch (socket_->receive((char*) &msgSize, sizeof(int))) {
//        case -1:
//            qDebug() << "error";
//            // error
//            break;
//        case 0:
//            qDebug() << "disconnect";
//            // disconnect
//            break;
//        default:
//            emit signalReceivedData(4096);
//            // data
//            char* buffer = new char[msgSize];
//            socket_->receive(buffer, msgSize);

//            // check if the wave header stuff is the same
//            // process buffer to buffer + HEADER_LENGTH - 1
//            if (audioPlayer_ == NULL) {
//                setupAudio(44100, 2, 16, 100000);
//            }


//            audioPlayer_->appendBuffer(buffer + sizeof(ComponentType) + HEADER_LENGTH, msgSize - HEADER_LENGTH);

//            // remove the memory since its now in the audio player
//            delete[] buffer;
//            break;
//        }

}
