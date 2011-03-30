#include "parseHdr.h"

ParseHdr::ParseHdr(QFILE * file) { 
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
    int i;
    QByteArray array;
    QString header
    
    array = new QByteArray(file, 44);
   
    setNumChan(array);
    setSampleRate(array);
    setByteRate(array);
    setBlockAlign(array);
    setBitsPerSample(array);
    setDataLen(array);
}

void ParseHdr::hdrParse(QByteArray * file) {
       
    setNumChan(file);
    setSampleRate(file);
    setByteRate(file);
    setBlockAlign(file);
    setBitsPerSample(file);
    setDataLen(file);
}

void ParseHdr::hdrParse(QFILE * file) {
    file.open(IO_ReadOnly);
    QByteArray array = file.read(44);
    file.close();

    setNumChan(array);
    setSampleRate(array);
    setByteRate(array);
    setBlockAlign(array);
    setBitsPerSample(array);
    setDataLen(array);
}

void ParseHdr::setNumChan(QByteArray * header) { 
    QChar temp = header[22];
    bool ok;
    numChan = temp.toInt(&ok, 16);   
    if(ok != 1) {
        printf("Error setting numChan");
    }
}

void ParseHdr::setSampleRate(QByteArray * header) {
    QByteArray temp = new QByteArray(4,0);
    int i;
    bool ok;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = header[24 + i];
    }
    sampleRate = temp.toInt(&ok, 16);
    if(ok != 1) {
        printf("Error setting sampleRate");
    }
}

void ParseHdr::setByteRate(QByteArray * header) {
    QByteArray temp = new QByteArray(4,0);
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = header[28 + i];
    }
    byteRate = temp.toInt(&ok, 16);
    if(ok != 1) {
        printf("Error setting byteRate");
    }
}

void ParseHdr::setBlockAlign(QByteArray * header) {
    QByteArray temp = new QByteArray(4,0);;
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[32 + i];
    }
    blockAlign = temp.toInt(&ok, 16);
    if(ok != 1) {
        printf("Error setting blockAlign");
    }
}

void ParseHdr::setBitsPerSample(QByteArray * header) {
    QByteArray temp = new QByteArray(4,0);
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[34 + i];
    }
    bitsPerSample  = temp.toInt(&ok, 16);
    if(ok != 1) {
        printf("Error setting bitsPerSample");
    }
}

void ParseHdr::setDataLen(QByteArray * header) {
    QByteArray temp = new QByteArray(4,0);
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[40 + i];
    }
    dataLen = temp.toInt(&ok, 16);
    if(ok != 1) {
        printf("Error setting dataLen");
    }
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
