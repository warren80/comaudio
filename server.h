#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "socket.h"
#include "dispatcher.h"
#include "logs.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(int port);
private:
    Socket * UDPServer_;
    Socket * TCPServer_;
    Dispatcher * dispatcher_;
signals:
public slots:
    void slotTransmitMessage(Message * msg);
    //void slotMessageFromSocket(Packet * packet);
};

#endif // SERVER_H
