#ifndef COMPONENTSTREAM_H
#define COMPONENTSTREAM_H

#include "component.h"
#include "audioplayer.h"
class ComponentStream : public Component {

public:
    ComponentStream(const Socket& socket);

    /**
      Create a new AudioPlayer.

      @param frequency Frequency of the wav.
      @param channels # of channels to play.
      @param sampleSize Size of each sample.
      @param bufferSize How much space to buffer incoming data to play.
      @author Nick Huber
      */
    void setupAudio(int frequency, int channels, int sampleSize, int bufferSize);
    void receiveData(char *data, int length);
    void transmitData(char *data, int length);
private:
    AudioPlayer* audioPlayer_;

};

#endif // COMPONENTSTREAM_H
