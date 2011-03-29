#ifndef PARSEWaveHdrD_H
#define PARSEHDR_H

#include <sstream>
#include <iostream>


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
    int getNumChan(WaveHdr *);
    int getSampleRate(WaveHdr *);
    int getByteRate(WaveHdr *);
    int getBlockAlign(WaveHdr *);
    int getBitsPerSample(WaveHdr *);
    int getDataLen(WaveHdr *);

private:
    WaveHdr * header;

    void hdrParse(char *);
    void setNumChan(char * file);
    void setSampleRate(char * file);
    void setByteRate(char *);
    void setBlockAlign(char *);
    void setBitsPerSample(char *);
    void setDataLen(char *);
};
#endif
