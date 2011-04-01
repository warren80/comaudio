#include <QDebug>

#include "componentstream.h"

ComponentStream::ComponentStream(const Socket& socket) : Component(kStream, socket), audioPlayer_(NULL) {

}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (audioPlayer_ != NULL) {
        delete audioPlayer_;
    }
    audioPlayer_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

void ComponentStream::receiveData(char *data, int length) {
    audioPlayer_->appendBuffer(data, length);
    // temp.
    //audioPlayer_->play();
}

void ComponentStream::transmitData(char *data, int length) {
    socket_.transmit(data, length);
}
