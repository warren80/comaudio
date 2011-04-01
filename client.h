#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include "socket.h"

class Client : public QThread
{
public:

    Client();
    ~Client();
    bool connect(in_addr_t address, uint16_t port) { return socket_->connect(address, port); };

protected:
    void run();

private:
    Socket* socket_;
    bool running_;

public slots:
    void slot_transmit(char* data, int length) { socket_->transmit(data, length); };
};

#endif // CLIENT_H
