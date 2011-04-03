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
    void signalSendFileList(Socket*);
    void serverVoiceMessage(char *, int);

public slots:
    void slotDisconnectStream();
    void slotPlayThisSong(QString songname);

private:
    void processClientMessage(Socket *clientSocket, char *buffer, int msgSize);
    void serverVoiceComponent(Socket * socket, char * buffer, int length);
    void startVoice();
    Socket* socket_;          /**< The socket information for the server. */
    bool running_;            /**< State of the thread, whether it should be running or not. */
    void startFileTransfer(QString, Socket *);
    QVector<Socket*> clients_;
};

#endif // SERVER_H
