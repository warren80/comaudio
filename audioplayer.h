/**
  @file
  Plays PCM audio through a buffering system.

  @bug At start of playback there is a 'pop', probably from WAVE header information.
  @author Nick Huber
  */

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QtMultimedia>

/**
  Plays PCM audio through a buffer.

  @author Nick Huber
  */
class AudioPlayer
{
public:

    /**
      Constructs the AudioPlayer object, creates a QAudioOutput as well as the associated QBuffer.
      @param bufferSize The amount of space to allocate inside the QAudioOutput for buffering data.
      @author Nick Huber
      */
    explicit AudioPlayer(int bufferSize = 64000);

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

private:
    QAudioOutput* audio_; /**< The audio output device. */
    QBuffer* buffer_;     /**< The buffer that audio_ reads from. */
};

#endif // AUDIOPLAYER_H
