#include "microphone.h"

/**
 * CONSTRUCTOR
 */
Microphone::Microphone() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat *format =  new QAudioFormat();

    //Setting audio format
    format->setFrequency(44100);
    format->setChannels(1);
    format->setSampleSize(16);
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
    connect(input_, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

void Microphone::stopRecording() {
    mic_->stop();
    recordFile_->close();
}

/**
 * SLOTS
 */
void Microphone::readData() {
    qint64 size = input_->size();
    if(size <= 0) {
        return;
    }
    input_->seek(size - dataWritten_);
    ba_ = new QByteArray(input_->read(size));
    emit sendVoice(ba_);
}

void Microphone::bytesWritten(qint64 data) {
    dataWritten_ = data;
}
