#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>

class Client : public QThread
{
public:
    Client();

protected:
    void run();
};

#endif // CLIENT_H
