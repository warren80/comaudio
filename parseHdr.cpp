#include "parseHdr.h"
#include <QDebug>

ParseHdr::ParseHdr(QFile * file) {
    hdrParse(file);
}

ParseHdr::ParseHdr(char * file) {
    hdrParse(file);
}

ParseHdr::ParseHdr(QByteArray * file) {
    hdrParse(file);
}

ParseHdr::~ParseHdr() { }

void ParseHdr::hdrParse(char * file) {
    QByteArray array;
    array = QByteArray(file, 44);
    setNumChan(&array);
    setSampleRate(&array);
    setByteRate(&array);
    setBlockAlign(&array);
    setBitsPerSample(&array);
    setDataLen(&array);

    header.bitsPerSample = bitsPerSample;
    header.dataLen = dataLen;
    header.numChan = numChan;
    header.sampleRate = sampleRate;
}

void ParseHdr::hdrParse(QByteArray * file) {
    setNumChan(file);
    setSampleRate(file);
    setByteRate(file);
    setBlockAlign(file);
    setBitsPerSample(file);
    setDataLen(file);
}

void ParseHdr::hdrParse(QFile * file) {
    if(!(file->open(QIODevice::ReadOnly))) {
        qDebug("File open failed");
    }
    QByteArray array = QByteArray(file->read(44));
    file->close();

    setNumChan(&array);
    setSampleRate(&array);
    setByteRate(&array);
    setBlockAlign(&array);
    setBitsPerSample(&array);
    setDataLen(&array);
}

void ParseHdr::setNumChan(QByteArray * header) {
    numChan = 0;
    numChan |= header->at(22);
    numChan = numChan << 8;
}

void ParseHdr::setSampleRate(QByteArray * header) {
    sampleRate = 0;
    QByteArray temp(4, '\0');
    int i;
    for(i = 0; i < 4; i++) {
        temp.push_front(header->at(27 - i));
    }
    memcpy(&sampleRate, temp, sizeof(int));
}

void ParseHdr::setByteRate(QByteArray * header) {
    byteRate = 0;
    QByteArray temp(4, '\0');
    int i;
    for(i = 0; i < 4; i++) {
        temp.push_front(header->at(31 - i));
    }
    memcpy(&byteRate, temp, sizeof(int));
}

void ParseHdr::setBlockAlign(QByteArray * header) {
    QByteArray temp(4, '\0');
    int i;
    for(i = 0; i < 2; i++) {
        temp.push_front(header->at(33 - i));
    }
    memcpy(&blockAlign, temp, sizeof(int));
}

void ParseHdr::setBitsPerSample(QByteArray * header) {
    QByteArray temp(4, '\0');
    int i;
    for(i = 0; i < 2; i++) {
        temp.push_front(header->at(35 - i));
    }
    memcpy(&blockAlign, temp, sizeof(int));
}

void ParseHdr::setDataLen(QByteArray * header) {
    QByteArray temp(4, '\0');
    int i;
    for(i = 0; i < 4; i++) {
        temp.push_front(header->at(43 - i));
    }
    memcpy(&dataLen, temp, sizeof(int));
}

int ParseHdr::getNumChan() {
    return numChan;
}

int ParseHdr::getSampleRate() {
    return sampleRate;
}

int ParseHdr::getByteRate() {
    return byteRate;
}

int ParseHdr::getBlockAlign() {
    return blockAlign;
}

int ParseHdr::getBitsPerSample() {
    return bitsPerSample;
}

int ParseHdr::getDataLen() {
    return dataLen;
}

WavHdr ParseHdr::getWavHdr() {
    return header;
}
