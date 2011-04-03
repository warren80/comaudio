#include "componentstream.h"
#include "componenttype.h"
#include "parseHdr.h"

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
                WaveHeader *wh = AudioPlayer::parseWaveHeader(buffer);
                // mohammed's not working wav parser
//                ParseHdr parser(buffer);
//                WavHdr hdr = parser.getWavHdr();

//                qDebug() << hdr.sampleRate << hdr.numChan << hdr.bitsPerSample << hdr.dataLen;

//                setupAudio(hdr.sampleRate, hdr.numChan, hdr.bitsPerSample, hdr.dataLen);
                setupAudio(wh->frequency, wh->channels, wh->bitsPerSample, 39379536);
                qDebug() << wh->frequency;
                qDebug() << wh->bitsPerSample;
                qDebug() << wh->channels;
            }

            emit signalReceivedData(STREAMPACKETSIZE);

            audioPlayer_->appendBuffer(buffer + HEADER_LENGTH, STREAMPACKETSIZE - HEADER_LENGTH);

        }

        delete[] buffer;
    }
}
