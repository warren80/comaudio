#include "microphone.h"

/**
 * CONSTRUCTOR
 */
Microphone::Microphone() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat *format =  new QAudioFormat();

    //Setting audio format
    format->setFrequency(32000);
    format->setChannels(1);
    format->setSampleSize(32);
    format->setCodec("audio/pcm");
    format->setByteOrder(QAudioFormat::LittleEndian);
    format->setSampleType(QAudioFormat::UnSignedInt);

    if(!info.isFormatSupported(*format)) {
        qDebug() << "Not acceptable format";
        *format = info.nearestFormat(*format);
        return;
    }

    mic_ = new QAudioInput(*format);
    recordFile_ = new QBuffer();
}

/**
 * HELPERS
 */
void Microphone::startRecording() {
    recordFile_->open(QIODevice::ReadWrite);
    mic_->start(recordFile_);
    input_ = recordFile_;

    connect(input_, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Microphone::stopRecording() {
    //QAudioOutput *temp = new QAudioOutput(*format_);
    mic_->stop();
    recordFile_->close();
    //recordFile_->open(QIODevice::ReadOnly);
    //temp->start(recordFile_);
}

/**
 * SLOTS
 */
void Microphone::readData() {
    qint64 size = input_->size();

    if(size <= 0) {
        return;
    }

    input_->seek(0);
    ba_ = input_->read(size);

    emit sendVoice(ba_.constData());
    ba_.clear();
}
