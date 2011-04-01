#ifndef COMPONENTSTREAM_H
#define COMPONENTSTREAM_H

#include "component.h"
#include "audioplayer.h"
#include <QFile>
#include "thread.h"

#define MULTICAST_IP "234.5.6.7"
#define MULTICAST_PORT 8888
#define HEADER_LENGTH 44
//#define STREAMPACKETSIZE 4096
//#define STREAMDATALENGTH  STREAMPACKETSIZE - HEADERLENGTH - 2

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

protected:
    void run();
//    void streamFileTransfer(QString fileNamePath);


private:
    AudioPlayer* audioPlayer_;
signals:
    void signalStreamFile();
};

//class StreamFileWorkerObject :  public QObject {
//Q_OBJECT
//private:
//    QString fileNamePath_;
//public:
//    StreamFileWorkerObject(QString fileNamePath);
//public slots:
//    void slotStartTransfer();
//signals:
//    void signalTransferDone();
//};

#endif // COMPONENTSTREAM_H
