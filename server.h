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
    void signalDispatcher(void *, int length);
public slots:
    void slotMessageFromSocket(void *, int length);
    void txMessage(void * buf, int length, int socketType, int socketDescriptor);
};

#endif // SERVER_H
