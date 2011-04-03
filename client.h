#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QObject>
#include "socket.h"
#include <QDebug>

class Client : public QThread
{

    Q_OBJECT

public:

    Client();
    ~Client();
    bool connect(in_addr_t address, uint16_t port) { return socket_->connect(address, port); }
    Socket* getSocket() {return socket_;}
protected:
    void run();

private:
    Socket* socket_;
    bool running_;

public slots:
    void slot_transmit(char* data, int length) { socket_->transmit(data, length); };
signals:
    void signalStopStream();
    void signalFileListReceived(char* data, int length);
    void signalShutdown();
    void signalSongName(QString songname);
};

#endif // CLIENT_H
