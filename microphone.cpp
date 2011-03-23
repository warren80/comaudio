#include "microphone.h"

Microphone::Microphone() {
    QAudioFormat format;
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();

    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    if(!info.isFormatSupported(format)) {
        qDebug() << "Not acceptable format";
        format = info.nearestFormat(format);
        return;
    }

    recordFile_ = new QFile("recording.raw");
    mic_ = new QAudioInput(format);

    recordFile_->open(QIODevice::WriteOnly);
    recordFile_->write("");
    recordFile_->close();

    connect(mic_, SIGNAL(notify()), this, SLOT(status()));
    connect(mic_, SIGNAL(stateChanged(QAudio::State)), this, SLOT(state(QAudio::State)));
}

Microphone::~Microphone() {

}

void Microphone::startRecording() {
    recordFile_->open(QIODevice::WriteOnly|QIODevice::Append);
    mic_->setNotifyInterval(1000);
    mic_->start(recordFile_);
}

void Microphone::stopRecording() {
    mic_->stop();
    recordFile_->close();

    /*
    QAudioFormat format;

    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput *echo_ = new QAudioOutput(format);
    recordFile_->open(QIODevice::ReadOnly);
    echo_->start(recordFile_);
    */
}

/**
 * SLOTS
 */
void Microphone::status() {

}

void Microphone::state(QAudio::State state) {
    switch(state) {
    case 0:
        qDebug() << "Active State";
        break;
    case 1:
        qDebug() << "Suspend State";
        break;
    case 2:
        qDebug() << "Stopped State";
        break;
    case 3:
        qDebug() << "Idle State";
        break;
    }
}
