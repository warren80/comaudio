#include <stdlib.h>
#include "parseHdr.h"
ParseHdr::ParseHdr() { }

ParseHdr::~ParseHdr() { }

void ParseHdr::hdrParse(char * file) {
    char * header;
    header = (char *) malloc (44);
    setNumChan(file);
    setSampleRate(file);
    setByteRate(file);
    setBlockAlign(file);
    setBitsPerSample(file);
    setDataLen(file);
}

void ParseHdr::setNumChan(char * file) { 
    ss << std::hex << file[22];
    ss >> numChan;
}

void ParseHdr::setSampleRate(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[24 + i];
    }
    ss << std::hex << temp;
    ss >> sampleRate;
}

void ParseHdr::setByteRate(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[28 + i];
    }
    ss << std::hex << temp;
    ss >> byteRate;
}

void ParseHdr::setBlockAlign(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[32 + i];
    }
    ss << std::hex << temp;
    ss >> blockAlign;
}

void ParseHdr::setBitsPerSample(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[34 + i];
    }
    ss << std::hex << temp;
    ss >> bitsPerSample;
}

void ParseHdr::setDataLen(char * file) {
    char temp[4];
    int i;
    for(i = 0; i < 4; i++) {
        temp[3 - i] = file[40 + i];
    }
    ss << std::hex << temp;
    ss >> dataLen;
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
