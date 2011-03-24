#include "microphone.h"

Microphone::Microphone() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();

    format_ =  new QAudioFormat();
    format_->setFrequency(8000);
    format_->setChannels(1);
    format_->setSampleSize(8);
    format_->setCodec("audio/pcm");
    format_->setByteOrder(QAudioFormat::LittleEndian);
    format_->setSampleType(QAudioFormat::UnSignedInt);

    if(!info.isFormatSupported(*format_)) {
        qDebug() << "Not acceptable format";
        *format_ = info.nearestFormat(*format_);
        return;
    }

    mic_ = new QAudioInput(*format_);
    echo_ = new QAudioOutput(*format_);
    recordFile_ = new QBuffer();

    connect(mic_, SIGNAL(notify()), this, SLOT(status()));
    connect(mic_, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateInput(QAudio::State)));
    connect(echo_, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateOutput(QAudio::State)));
}

Microphone::~Microphone() {
}

void Microphone::startRecording() {
    recordFile_->open(QIODevice::WriteOnly|QIODevice::Append);
    mic_->start(recordFile_);
    mic_->setNotifyInterval(100);
}

void Microphone::stopRecording() {
    mic_->stop();
    echo_->stop();
}

/**
 * SLOTS
 */
void Microphone::status() {
    QBuffer *temp = new QBuffer(recordFile_);
    qDebug() << temp->read(1024).data();

    emit sendVoice(temp->read(1024).data());
}

void Microphone::stateInput(QAudio::State state) {
    switch(state) {
    case 0:
        qDebug() << "Input - Active State";
        break;
    case 1:
        qDebug() << "Input - Suspend State";
        break;
    case 2:
        qDebug() << "Input - Stopped State";
        break;
    case 3:
        qDebug() << "Input - Idle State";
        break;
    }
}

void Microphone::stateOutput(QAudio::State state) {
    switch(state) {
    case 0:
        qDebug() << "Output - Active State";
        break;
    case 1:
        qDebug() << "Output - Suspend State";
        break;
    case 2:
        qDebug() << "Output - Stopped State";
        break;
    case 3:
        qDebug() << "Output - Idle State";
        break;
    }
}
