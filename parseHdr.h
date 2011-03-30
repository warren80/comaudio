#ifndef PARSEHDR_H
#define PARSEHDR_H

#include <QFile>
#include <QChar>
#include <QByteArray>
#include <QString>

class ParseHdr {

public:
    int getNumChan();
    int getSampleRate();
    int getByteRate();
    int getBlockAlign();
    int getBitsPerSample();
    int getDataLen();

    explicit  ParseHdr(QFile *);
    explicit  ParseHdr(char *);
    ~ParseHdr();
private:
    stringstream ss;
    int numChan;
    int sampleRate;
    int byteRate;
    int blockAlign;
    int bitsPerSample;
    int dataLen;
    unsigned char *extra;
    int extralen;
    
    void hdrParse(char *);
    void hdrParse(FILE *);
    void hdrParse(QByteArray *);
    void setNumChan(QByteArray *);
    void setSampleRate(QByteArray *);
    void setByteRate(QByteArray *);
    void setBlockAlign(QByteArray *);
    void setBitsPerSample(QByteArray *);
    void setDataLen(QByteArray *);
};
#endif
