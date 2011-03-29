#include "parseHdr.h"

ParseHdr::ParseHdr() {
    header = new WaveHdr();    
}

ParseHdr::~ParseHdr() { }

void ParseHdr::hdrParse(char * file) {
    setNumChan(file);
    setSampleRate(file);
    setByteRate(file);
    setBlockAlign(file);
    setBitsPerSample(file);
    setDataLen(file);
}

void ParseHdr::setNumChan(char * file) { 
    ss << hex << file[22];
    ss >> header->numChan;
}

void ParseHdr::setSampleRate(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[24 + i];
    }
    ss << hex << temp;
    ss >> header->sampleRate;
}

void ParseHdr::setByteRate(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[28 + i];
    }
    ss << hex << temp;
    ss >> header->byteRate;
}

void ParseHdr::setBlockAlign(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[32 + i];
    }
    ss << hex << temp;
    ss >> header->blockAlign;
}

void ParseHdr::setBitsPerSample(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[34 + i];
    }
    ss << hex << temp;
    ss >> header->bitsPerSample;
}

void ParseHdr::setDataLen(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[40 + i];
    }
    ss << hex << temp;
    ss >> header->dataLen;
}

int ParseHdr::getNumChan(WaveHdr * header) {
    return header->numChan;
}
int ParseHdr::getSampleRate(WaveHdr * header) {
    return header->sampleRate;
}
int ParseHdr::getByteRate(WaveHdr * header) {
    return header->byteRate;
}
int ParseHdr::getBlockAlign(WaveHdr * header) {
    return header->blockAlign;
}
int ParseHdr::getBitsPerSample(WaveHdr * header) {
    return header->bitsPerSample;
}
int ParseHdr::getDataLen(WaveHdr * header) {
    return header->dataLen;
}
