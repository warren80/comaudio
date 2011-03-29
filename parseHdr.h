#ifndef PARSEHDR_H
#define PARSEHDR_H

#include <iostream>
#include <sstream>
using namespace std;


/*typedef struct WaveHdr {
    int numChan;
    int sampleRate;
    int byteRate;
    int blockAlign;
    int bitsPerSample;
    int dataLen;
    unsigned char *extra;
    int extralen;
} WaveHdr;
*/
class ParseHdr {

public:
    int getNumChan();
    int getSampleRate();
    int getByteRate();
    int getBlockAlign();
    int getBitsPerSample();
    int getDataLen();

    explicit  ParseHdr();
    ~ParseHdr();
private:
    //WaveHdr * header;
    stringstream ss;
    istringstream ins;
    ostringstream outs;
    int numChan;
    int sampleRate;
    int byteRate;
    int blockAlign;
    int bitsPerSample;
    int dataLen;
    unsigned char *extra;
    int extralen;
    
    void hdrParse(char *);
    void setNumChan(char *);
    void setSampleRate(char *);
    void setByteRate(char *);
    void setBlockAlign(char *);
    void setBitsPerSample(char *);
    void setDataLen(char *);
};
#endif
