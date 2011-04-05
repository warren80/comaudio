#include "microphone.h"
#include <errno.h>

/**
 * CONSTRUCTOR
 */
Microphone::Microphone() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat *format =  new QAudioFormat();

    recordFile_ = 0;
    ba_ = 0;
    input_ = 0;
    mic_ = 0;

    //Setting audio format
    format->setFrequency(44100);
    format->setChannels(2);
    format->setSampleSize(16);
    format->setCodec("audio/pcm");
    format->setByteOrder(QAudioFormat::LittleEndian);
    format->setSampleType(QAudioFormat::SignedInt);

    if(!info.isFormatSupported(*format)) {
        QString exception("Microphone constructor failure: ");
        exception.append(strerror(errno));
        throw exception;
    }

    mic_ = new QAudioInput(*format);
    recordFile_ = new QBuffer();
}

Microphone::~Microphone () {
    if (recordFile_ != 0) {
        delete recordFile_;
    }
    if (ba_ != 0) {
        delete ba_;
    }
    if (input_ != 0) {
        delete input_;
    }
    if (mic_ != 0) {
        delete mic_;
    }
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
    //qDebug() << "read:" << data;
    dataWritten_ = data;
}
