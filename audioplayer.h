/**
  @file
  Plays PCM audio through a buffering system.

  @bug At start of playback there is a 'pop', probably from WAVE header information.
  @author Nick Huber
  */

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QtMultimedia>

struct WaveHeader {
    int frequency;
    int channels;
    int bitsPerSample;
};

/**
  Plays PCM audio through a buffer.

  @author Nick Huber
  */
class AudioPlayer {
public:

    /**
      Constructs the AudioPlayer object, creates a QAudioOutput as well as the associated QBuffer.
      @param bufferSize The amount of space to allocate inside the QAudioOutput for buffering data.
      @author Nick Huber
      */
    explicit AudioPlayer(int frequency, int channels, int sampleSize, int bufferSize = 64000);

    /**
      Appends data to the buffer to be played.
      @param data The data in bytes (chars).
      @param length The size of the data.
      @author Nick Huber
      */
    void appendBuffer(char* data, int length);

    /**
      Resume from being paused.
      @author Nick Huber
      */
    void play();

    /**
      Pause playback. Data can still be buffered.
      @author Nick Huber
      */
    void pause();
    static WaveHeader * parseWaveHeader(char hdr[44]);
private:
    QAudioOutput* audio_; /**< The audio output device. */
    QIODevice* buffer_;     /**< The buffer that audio_ reads from. */
    int frequency_;
    int channels_;
    int sampleSize_;
    int bufferSize_;
};

#endif // AUDIOPLAYER_H
