#include "audioplayer.h"

#include <QtMultimedia>

AudioPlayer::AudioPlayer(int bufferSize) {
    QAudioFormat format;
    // Set up the format, eg.
    format.setFrequency(16000);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    audio_ = new QAudioOutput(format);
    audio_->setBufferSize(bufferSize);
    //connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
    buffer_ = (QBuffer*) audio_->start();
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
