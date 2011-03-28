#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>

#include "socket.h"

class Client : public QThread
{
public:
    Client();
    ~Client();

protected:
    void run();

private:
    Socket* socket_;
    bool running_;
};

#endif // CLIENT_H
