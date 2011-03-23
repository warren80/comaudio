#ifndef SOCKET_H
#define SOCKET_H

#include <QString>

#include <stdio.h>
#include <QDebug>
#include <QtGui>
#include <QFile>

#include "logs.h"
#include "packet.h"

#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#endif

#define BUFSIZE 1024
#define IPADDRSIZE 16
#define ALIASSIZE 32
#define LENGTHSIZE 4

/**
* A generic socket class where the socket will be created or destroyed. This is
* also where messages are to be transmitted or received.
*
* This is a multicast socket that uses select for efficiency and performance.
*/
//THIS NEEDS TO BE REMOVED using message and packet which are declared in packet.h
struct MessageStruct {
    int mesgType; //0 text, 1 voice, 2 stream, 3 file
    char ipAddr[16];
    char alias[32];
    char data[BUFSIZE - IPADDRSIZE - ALIASSIZE];
};

class Socket : public QObject {
    Q_OBJECT
public:
    /**
    * The socket class constructor where some variables are initialized. Some
    * connections are also done here.
    *
    * @author Karl Castillo, Warren Voelkl
    * @arg type - the type of service (client or server)
    * port - the port in which the socket will be bound to
    */
    Socket(int type, int port);

    /**
    * Call this method to send a Packet to the client.
    *
    * @author Warren Voelkl
    * @arg pckt - the struct that will be transmitted
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int tx(Packet *pckt);
    /**
    * An overload of the tx method.
    *
    * @author Warren Voelkl
    * @arg mesg - the struct that will be trasmitted
    * length - the length of the struct to be transmitted
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int tx(Packet *pckt, int socketID);
    /**
    * An overload of the tx method.
    *
    * @author Karl Castillo
    * @arg mesg - the struct that will be trasmitted
    * length - the length of the struct to be transmitted
    * socketDescriptor - the socket where the struct is to be transmitted
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int tx(Packet *pckt, char * ipAddr);
    /**
    * This is a blocked method and will only unblock if a message is received or
    * other socket operation is going on.
    *
    * @author Karl Castillo, Warren Voelkl
    * @arg mesg - the struct where the message received will be stored
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int rx(MessageStruct *mesg);
    /**
    * Call this method to close the socket descriptor.
    *
    * @author Karl Castillo, Warren Voelkl
    */
    void closeSocket();
public slots:
    /**
    * This method will create a socket that will be used by the client. This
    * method will only be called if the user specified the application to be
    * client.
    *
    * @author Warren Voelkl
    * @arg str - the type of protocol to be used (TCP or UDP)
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    void SetAsClient(const char *str);
    /**
    * This method will create a socket that will be used by the server. This
    * method will only be called if the user specified the application to be
    * server.
    *
    * @author Warren Voelkl
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    void SetAsServer();
private:
    /**
    * Create a TCP message and emit it
    *
    * @author Warren Voelkl
    */
    void createMessage(void * buffer, int socketID);
    /**
    * Create a UDP message and emit it
    *
    * @author Warren Voelkl
    */
    void createMessage(void * buffer, char * ipAddr);
    /**
    * Call this method to create a TCP socket drescriptor.
    *
    * @author Warren Voelkl
    */
    void createTCPSocket();
    /**
    * Call this method to create a UDP socket descriptor.
    *
    * @author Warren Voelkl
    */
    void createUDPSocket();
    /**
    * Call this method to start listening for clients as a TCP server. This will
    * be a multicast server that makes use of select.
    *
    * @author Karl Castillo, Warren Voelkl
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int TCPServer();
    /**
    * Not implemented.
    */
    int UDPServer();
    /**
    * Call this method to setup the socket descriptor.
    *
    * @author Warren Voelkl
    * @arg str - determines whether the socket is for a server or for a client
    * @return 0 - if no error occurs
    * -1 - if an error occurs
    */
    int SetupSocket(const char *str);
    /**
    * This method is called to send the message received by the server to the
    * other clients.
    *
    * @author Warren Voelkl
    * @arg maxi - the max index of the client array
    * client - the array of clients
    * recieveSocketDescriptor - the socket descriptor of the client that
    * sent the message
    * mesg - the struct that is to be sent to the clients
    */
    void writeToEveryoneElse(int maxi, int client[FD_SETSIZE],
             int recieveSocketDescriptor, MessageStruct * mesg);
    /**
    * The file descriptor of the server log.
    */
    QFile *log_;
    /**
    * The file descriptor of the error log.
    */
    QFile *errorLog_;
    /**
    * The length of the struct that will be sent.
    */
    int buflen_;
    /**
    * The type of socket (TCP or UDP).
    */
    int socketType_;
    /**
    * The port where the socket will be bound to.
    */
    int sPort_;
    /**
    * The sockaddrs for the client and server.
    */
    struct sockaddr_in client_, server_;
    /**
    * The length of the server sockaddr.
    */
    int serverLength_;
    /**
    * The socket descriptor of the application.
    */
    int socketDescriptor_;

    /**
     * Creates a buffer for transmission from a packet
     */
    char * createBuffer(Packet * pckt);

signals:
    void signalPacketRecieved(Message * msg);
    void signalSocketClosed(int socketID);
    /**
    * Emitted if a client connects to the server.
    */
};

#endif
