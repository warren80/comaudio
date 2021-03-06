#include "componentstream.h"
#include "componenttype.h"

ComponentStream::ComponentStream() : Component(new Socket(kUDP)), audioPlayer_(NULL) {
    if (!socket_->clientJoinMCast(inet_addr(MULTICAST_IP), htons(MULTICAST_PORT))) {
        emit signalPrintF(QString("error joining multicast"));
    }
}

ComponentStream::~ComponentStream() {
    if (audioPlayer_ != 0) {
        delete audioPlayer_;
    }
}

void ComponentStream::setupAudio(int frequency, int channels, int sampleSize, int bufferSize) {
    if (audioPlayer_ != NULL) {
        delete audioPlayer_;
        audioPlayer_  = 0;
    }
    audioPlayer_ = new AudioPlayer(frequency, channels, sampleSize, bufferSize);
}

void ComponentStream::resetAudio() {
    delete audioPlayer_;
    audioPlayer_ = NULL;
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
                WaveHeader *wh = AudioPlayer::parseWaveHeader(buffer);
                setupAudio(wh->frequency, wh->channels, wh->bitsPerSample, 39379536);
                emit signalSongData(wh);
            }

            audioPlayer_->appendBuffer(buffer + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH);

        }
        if (buffer != 0) {
            delete[] buffer;
            buffer = 0;
        }
    }
}
