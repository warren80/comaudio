#ifndef SERVER_H
#define SERVER_H

#include <QThread>

#include "socket.h"
#include "dispatcher.h"

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

      @param port Port # to listen to.
      @author Nick Huber
      */
    Server(int port);

protected:
    /**
      Overwritten run method from QThread, select server runs from here
      */
    void run();

private:
    Socket socket_;         /**< The socket information for the server. */
    Dispatcher dispatcher_; /**< Dispatch recieved messages to the right component.*/
    bool running_;          /**< State of the thread, whether it should be running or not. */

};

#endif // SERVER_H
