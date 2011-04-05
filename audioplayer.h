#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

/**
  @file
  Plays PCM audio through a buffering system.

  @author Nick Huber
  */

#include <QtMultimedia>

struct WaveHeader {
    int frequency;      /**< The sample rate of the pcm audio file */
    int channels;       /**< The number of channels in a audio file */
    int bitsPerSample;  /**< The number of bits to describe each audio sample */
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
      Destructor for the audio player.

      @author Nick Huber
      */
    ~AudioPlayer() {
        delete buffer_;
        delete audio_;
    }

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
    /**
     * Reads the header from a wav pcm audio file
     * and then fills a struct to be used in setting up the
     * audio player.
     *
     * @author Warren Voelkl
     */
    static WaveHeader * parseWaveHeader(char hdr[44]);
private:
    QAudioOutput* audio_;   /**< The audio output device. */
    QIODevice* buffer_;     /**< The buffer that audio_ reads from. */
};

#endif // AUDIOPLAYER_H
