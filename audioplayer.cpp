#include "audioplayer.h"
#include <QFile>
#include <QtMultimedia>
#include "winsock2.h"

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

void AudioPlayer::appendBuffer(char* data, int length) {
    buffer_->write(data, length);
}

void AudioPlayer::play() {
    audio_->resume();
}

void AudioPlayer::pause() {
    audio_->suspend();
}

void swapBytes(char *a, char *b) {
    char c;
    c = *a;
    *a = *b;
    *b = c;
}

WaveHeader * AudioPlayer::parseWaveHeader(char hdr[44]) {
    WaveHeader *wh = new WaveHeader();
    short bps, channel;

    memcpy(&bps, hdr + 34, 2);
    memcpy(&channel, hdr + 22, 2);
    memcpy(&wh->frequency, hdr + 24,4);
    wh->bitsPerSample = bps;
    wh->channels = channel;

    //qDebug() << wh->bitsPerSample;
    //qDebug() << wh->channels;
    //qDebug() << wh->frequency;
    return wh;
}
