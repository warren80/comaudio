#include "componentstream.h"

ComponentStream::ComponentStream(const Socket& socket) : Component(kStream, socket) {

}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (player_ != NULL) {
        delete player_;
    }
    player_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

void ComponentStream::receiveData(char *data, int length) {
    player_->appendBuffer(data, length);
}
