#ifndef COMPONENTSTREAM_H
#define COMPONENTSTREAM_H

#include "component.h"
#include "audioplayer.h"
#include <QFile>
#include "thread.h"
#include "packet.h"

#define MULTICAST_IP                    "234.5.6.7"
#define MULTICAST_PORT                  8888
#define HEADER_LENGTH                   44
#define HEADER_WAVE_AND_PACKETHEADER    (HEADER_LENGTH + sizeof(Packet) - sizeof(char *))
#define STREAMPACKETSIZE                8192
#define STREAMDATALENGTH                (STREAMPACKETSIZE - HEADER_WAVE_AND_PACKETHEADER)

class ComponentStream : public Component {
Q_OBJECT
public:
    ComponentStream();

    /**
      Create a new AudioPlayer.

      @param frequency Frequency of the wav.
      @param channels # of channels to play.
      @param sampleSize Size of each sample.
      @param bufferSize How much space to buffer incoming data to play.
      @author Nick Huber
      */
    void setupAudio(int frequency, int channels, int sampleSize, int bufferSize);

    void resetAudio();

protected:
    void run();
//    void streamFileTransfer(QString fileNamePath);


private:
    AudioPlayer* audioPlayer_;
signals:
    void signalStreamFile();
    void signalReceivedData(int);
    void signalSongData(WaveHeader* header);
};

#endif // COMPONENTSTREAM_H
