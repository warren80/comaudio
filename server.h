#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include "dispatcher.h"

class Server
{
public:
    Server(int port);
private:
    Socket socket_; /**< The socket information for the server. */
signals:
    void signalServerStart();
public slots:
    void slotTransmitMessage(Message * msg);
    //void slotMessageFromSocket(Packet * packet);

};

#endif // SERVER_H
