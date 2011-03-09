#ifndef SOCKET_H
#define SOCKET_H

#include "includes.h"
#include "packet.h"

class Socket : public QObject {
    Q_OBJECT
public:
    Socket(int type, int port, int packetSize);
    int SetAsClient(const char * str);
    int SetAsServer();

    int tx(const char * str, int length);
    int tx(const QString str);
    int tx(const char * str, int length, int socketDescriptor);
    int rx(char * str);
    void closeSocket();
private:
    void createTCPSocket();
    void createUDPSocket();
    int TCPServer();
    int UDPServer();
    int SetupSocket(const char * str);

    int buflen_;
    int socketType_;
    int sPort_;
    //struct sockaddr_in client_, server_;
    int serverLength_;
    int socketDescriptor_;

signals:
    void SignalClientConnected(char * client);
    void SignalClientDisconnected(char * client);
    void SignalPacketReceived(void *);
};
#endif
