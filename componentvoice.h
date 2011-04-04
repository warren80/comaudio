#ifndef COMPONENTVOICE_H
#define COMPONENTVOICE_H

#include <QThread>
#include <QDebug>

#include "audioplayer.h"
#include "socket.h"
#include "microphone.h"
#include "thread.h"
#include "component.h"

/**
  Voice function that uses a mutlicast peer to all style.
  a voice client connects as a multicast session and transmits to all others
  Then recieves from everyone else in the same session

  @author Warren Voelkl
  */
class ComponentVoice : public Component
{
    Q_OBJECT

public:
    /**
      ComponentVoice constructor
      @author Warren Voelkl
      */
    ComponentVoice(Socket* socket);

    /**
      ComponentVoice
      @author Warren Voelkl
      */
    ~ComponentVoice();




protected:
    /**
      Overwritten run method from QThread, recieve loop runs here.
      @note Do not run this directly, call QThead::start() instead.
      @author Warren Voelkl
      */
    void run();
public slots:
    void transmitVoice(QByteArray * ba);
    void slotStopVoiceComponent();
    /**
     * Parses incoming data and gives it to the audioplayer for playback
     *
     * @author Warren Voelkl
     */
    void receiveData(char* data, int length);
signals:

private:
    AudioPlayer *ap_;       /**< The audio player for the received data*/
    Socket* socket_;        /**< The socket information for the ComponentVoice. */
    Microphone* mic_;       /**< The Microphone Reader */
    Thread* micThread_;      /**< The thread for running the microphone */
};

#endif // SERVER_H
