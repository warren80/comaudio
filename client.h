#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QObject>
#include "socket.h"
#include <QDebug>

class Client : public QThread
{

    Q_OBJECT

public:
    /**
     * Initial constructor for the client object
     * sets up the socket for a tcp connection to the server
     *
     * @author Nick Huber
     */
    Client();

    /**
      Destructor for the client.

      @author Nick Huber
      */
    ~Client();

    /**
     * Starts the connection to the server on the current socket
     */
    bool connect(in_addr_t address, uint16_t port) { return socket_->connect(address, port); }

    /**
     * @returns the client socket for use in the main window and client components
     * @author Warren Voelkl
     */
    Socket* getSocket() {return socket_;}
protected:
    /**
     * The loop where the client blocks while waiting to read from the socket.
     *
     * dispatches messages to the corresponding components.
     * Warren Voelkl, Nick Huber
     */
    void run();

private:
    Socket* socket_; /**< The tcp socket for the connection to the server */
    bool running_; /**< boolean to determine current state of the client object */

public slots:
    /**
     * Transmits data on the client socket
     * @author Nick Huber
     */
    void slot_transmit(char* data, int length) { socket_->transmit(data, length); }
signals:
    /**
      * The stream has stopped.
      *
      * @author Nick Huber
      */
    void signalStopStream();
    /**
      * Received list of files.
      *
      * @param data | seperated list of files.
      * @param length length of the data.
      *
      * @author Nick Huber
      */
    void signalFileListReceived(char* data, int length);
    /**
      * Client has shutdown.
      *
      * @author Nick Huber
      */
    void signalShutdown();
    /**
      * Song name received.
      *
      * @param songname Name of the song playing.
      *
      * @author Nick Huber
      */
    void signalSongName(QString songname);
    /**
      * Voice message received.
      *
      * @param data Voice message data.
      * @param length Amount of voice data.
      *
      * @author Nick Huber
      */
    void signalVoiceMessage(char* data, int length);
    /**
      * File transfer data received.
      *
      * @param data transfer message data.
      * @param length Amount of transferred data.
      *
      * @author Nick Huber
      */
    void signalFileDataReceived(char* data, int length);
    /**
      * File transfer finished.
      *
      * @author Nick Huber
      */
    void signalFileFinished();
    /**
      * Voice communication stopped
      *
      * @author Nick Huber
      */
    void signalStopVoiceMessage();
};

#endif // CLIENT_H
