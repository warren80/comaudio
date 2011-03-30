#ifndef COMPONENTVOICE_H
#define COMPONENTVOICE_H

#include <QThread>
#include <QDebug>

#include "audioplayer.h"
#include "socket.h"
#include "microphone.h"

#include "componentvoice.h"

#define MULTICASTIP "234.5.6.7"
#define MULTICASTPORT 8888

/**
  Voice function that uses a mutlicast peer to all style.
  a voice client connects as a multicast session and transmits to all others
  Then recieves from everyone else in the same session

  @author Warren Voelkl
  */
class ComponentVoice : public QThread
{
    Q_OBJECT

public:
    /**
      ComponentVoice constructor listens to a multicast session
      @author Warren Voelkl
      */
    ComponentVoice();

    /**
      ComponentVoice, shuts down socket and closes recieve loop
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

private:
    Socket* socket_;        /**< The socket information for the ComponentVoice. */
    AudioPlayer* ap_;       /**< The audio player */
    Microphone* mic_;       /**< The Microphone Reader */
};

#endif // SERVER_H
