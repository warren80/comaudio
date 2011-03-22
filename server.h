#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "socket.h"
#include "dispatcher.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
private:
    SocketClass UDPServer_;
    SocketClass TCPServer_;
    Dispatcher dispatcher_;
signals:
    void signalDispatcher(void *, int length);
public slots:
    void slotMessageFromSocket(void *, int length);
    void txMessage(void * buf, int length, int socketType);
};

#endif // SERVER_H
