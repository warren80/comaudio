#include "audioplayer.h"
#include <QFile>
#include <QtMultimedia>

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
