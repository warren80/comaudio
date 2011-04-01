#ifndef PARSEHDR_H
#define PARSEHDR_H

#include <QFile>
#include <QChar>
#include <QByteArray>
#include <QString>
#include <string.h>

struct WavHdr {
    int numChan;
    int sampleRate;
    int bitsPerSample;
    int dataLen;
} WavHdr;

class ParseHdr {

public:
    explicit ParseHdr(QFile * file);
    explicit ParseHdr(char * file);
    explicit ParseHdr(QByteArray * file);
    ~ParseHdr();

    int getNumChan();
    int getSampleRate();
    int getByteRate();
    int getBlockAlign();
    int getBitsPerSample();
    int getDataLen();
    WavHdr getWavHdr();

private:
    int numChan;
    int sampleRate;
    int byteRate;
    int blockAlign;
    int bitsPerSample;
    int dataLen;

    WavHdr header;

    void hdrParse(QFile *);
    void hdrParse(char *);
    void hdrParse(QByteArray *);

    void setNumChan(QByteArray *);
    void setSampleRate(QByteArray *);
    void setByteRate(QByteArray *);
    void setBlockAlign(QByteArray *);
    void setBitsPerSample(QByteArray *);
    void setDataLen(QByteArray *);
};
#endif
