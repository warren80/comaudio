#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>

#include "socket.h"

class Client : public QThread
{
public:
    Client(char *ipAddr, int port, QString alias);

protected:
    void run();

private:
    QString alias_;
    Socket socket_;
    bool running_;
};

#endif // CLIENT_H
