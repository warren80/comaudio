#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QVector>
#include "socket.h"

/**
  Server class that creates a socket then creates a thread to receive incoming TCP
  connections through the use of the select function.

  @author Nick Huber
  */
class Server : public QThread
{
    Q_OBJECT

public:
    /**
      Server constructor, takes a port to listen to.

      @param port Port # to listen to in network byte ordering.
      @param backlog Number of connections to queue.
      @author Nick Huber
      */
    explicit Server(int port, int backlog = 5);

    /**
      Server destructor, shuts down socket and closes the server loop if running.

      @author Nick Huber
      */
    ~Server();

protected:
    /**
      Overwritten run method from QThread, TCP select server runs from here.

      @note Do not run this directly, call QThead::start() instead.

      @author Nick Huber
      */
    void run();
signals:
    void signalStreamFile();
    void signalStopVoiceComponent();
    void serverVoiceMessage(char *, int);
    void signalStartComponentVoice();
    void signalClientConnect(Socket* socket);

public slots:
    /** Transmits and empty packet to indicate the end of a file stream */
    void slotDisconnectStream();
    /**
     *  Transmits a packet which tells the name of the sont that is playing
     */
    void slotPlayThisSong(QString songname);

private:
    /**
     * Creates a new thread for the voice audio component to run in.
     *
     * @author Warren Voelkl
     */
    void setupVoiceComponent(Socket * socket);

    /**
     * Creates a new thread for the file transfer component to run in.
     *
     * @author Warren Voelkl
     */
    void startFileTransfer(QString, Socket *);

    /**
     * Reads the message type from the buffer and then calls the correct fn.
     *
     * @author Warren Voelkl
     */
    void processClientMessage(Socket *clientSocket, char *buffer, int msgSize);

    /**
     * Contains the logic for the voice component.
     *
     * Either starts, stops or passes a buffer to the voice componet
     */
    void serverVoiceComponent(Socket * socket, char * buffer, int length);

    /**
     * Creates a new thread for the file transfer component to run in.
     *
     * @author Warren Voelkl
     */
    void startVoice();

    Socket* socket_;          /**< The socket information for the server. */
    bool running_;            /**< State of the thread, whether it should be running or not. */
    QVector<Socket*> clients_;/**< Container for all socket objects. */
};

#endif // SERVER_H
