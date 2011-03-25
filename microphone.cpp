#include "microphone.h"

Microphone::Microphone() {
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();

    format_ =  new QAudioFormat();
    format_->setFrequency(32000);
    format_->setChannels(1);
    format_->setSampleSize(32);
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
}

Microphone::~Microphone() {
}

void Microphone::startRecording() {
    recordFile_->open(QIODevice::ReadWrite);
    mic_->start(recordFile_);
    input_ = recordFile_;

    connect(input_, SIGNAL(bytesWritten(qint64)), this, SLOT(dataWritten(qint64)));
    connect(input_, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Microphone::stopRecording() {
    QAudioOutput *temp = new QAudioOutput(*format_);
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

    //qDebug() << "Ready: " << QString::number(size);

    if(size <= 0) {
        qDebug() << "Returning";
        return;
    }

    input_->seek(0);
    ba_ = input_->read(size);
    if(ba_.isEmpty()) {
        qDebug() << "ba is empty";
    }

    /*
    for(int i = 0; i < ba_.size(); i++) {
        qDebug("%0X", ba_.at(i));
    }
    */

    emit sendVoice(ba_.constData());
    qDebug() << ba_.constData();
    ba_.clear();

    //qDebug() << "[1] ba.data size: " << QString::number(qstrlen(ba_->constData()));
    //qDebug() << "[1] ba size: " << QString::number(ba_->size());
}

void Microphone::dataWritten(qint64 x) {
    //qDebug() << "Written: " << QString::number(x);
}

void Microphone::status() {

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
