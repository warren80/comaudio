
#include <QFile>
#include <QtMultimedia>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "audioplayer.h"


AudioPlayer::AudioPlayer(int frequency, int channels, int sampleSize, int bufferSize) {
    QAudioFormat format;
    // Set up the format, eg.
    format.setFrequency(frequency);
    format.setChannels(channels);
    format.setSampleSize(sampleSize);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio_ = new QAudioOutput(format);
    audio_->setBufferSize(bufferSize);

    buffer_ = audio_->start();

}
AudioPlayer::~AudioPlayer() {
    qDebug() << "delete AudioPlayer";
    delete buffer_;
    delete audio_;
}

void AudioPlayer::appendBuffer(char* data, int length) {
    buffer_->write(data, length);
}

void AudioPlayer::play() {
    audio_->resume();
}

void AudioPlayer::pause() {
    audio_->suspend();
}

WaveHeader * AudioPlayer::parseWaveHeader(char hdr[44]) {
    WaveHeader *wh = new WaveHeader();
    short bps, channel;

    memcpy(&bps, hdr + 34, 2);
    memcpy(&channel, hdr + 22, 2);
    memcpy(&wh->frequency, hdr + 24,4);
    wh->bitsPerSample = bps;
    wh->channels = channel;
    return wh;
}
