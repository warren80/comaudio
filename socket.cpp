#include "socket.h"

#ifndef _WIN32
#define closesocket close
#include <sys/types.h>
#endif

#ifdef _WIN32
typedef int socklen_t;
#endif

#include "socket.h"
#include <QDateTime>
#include <errno.h>

Socket::Socket(int type, int port) {
    sPort_ = port;
    socketType_ = type;
    buflen_ = sizeof(MessageStruct);

    log_ = new QFile("log");
    errorLog_ = new QFile("errorLog");

    errorLog_->open(QIODevice::WriteOnly);

    errorLog_->write(QString("Error Log ("
                       + QDateTime::currentDateTime().toString()
                       + ")").toLatin1().data());

    errorLog_->close();

    switch (socketType_) {
    case TCP:
        createTCPSocket();
        break;
    case UDP:
        createUDPSocket();
        break;
    default:
        qDebug() << "Socket(): invalid socket type";
        return;
    }
}

void Socket::SetAsServer() {
    if (SetupSocket(0) != 1) {
        qDebug() << "SetAsServer(): SetupSocket";
        return;
    }

    if (socketType_ == UDP) {
        UDPServer();
        return;
    }
    if (socketType_ == TCP) {
        TCPServer();
        return;
    }
    return;
}

int Socket::TCPServer() {
    int maxfd, maxi, nready, bytesToRead, n, i;
    socklen_t clientLength;
    int client[FD_SETSIZE];
    int newSocketDescriptor, recieveSocketDescriptor;
    struct sockaddr_in clientAddr;
    fd_set allset, rset;

    MessageStruct * tempMesg;
    MessageStruct * mesg = new MessageStruct();

    if(!log_->open(QIODevice::WriteOnly)) {
        return -1;
    }

    log_->write(QString("Server Log (Started"
                       + QDateTime::currentDateTime().toString()
                       + ")").toLatin1().data());

    log_->close();

    if(bind(socketDescriptor_, (struct sockaddr *) &server_,
            sizeof(server_)) == -1) {
        qDebug() << "TCPServer(): bind";
        writeToLog(errorLog_, QString("\nTCPServer(): bind - Errno(" + QString::number(errno)
                                      + " ~ " + QTime::currentTime().toString() + ")"));
        return -1;
    }

    qDebug() << "Listening for clients";
    listen(socketDescriptor_, FD_SETSIZE);
    maxfd = socketDescriptor_;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET((unsigned)socketDescriptor_, &allset);

    while (1) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(socketDescriptor_, &rset)) {
            clientLength = sizeof(clientAddr);
            if ((newSocketDescriptor = accept(socketDescriptor_,
                                              (struct sockaddr *) &clientAddr,
                                              &clientLength)) == -1) {
                qDebug() << "TCPServer(): accept";
                writeToLog(errorLog_, QString("\nTCPServer(): accept - Errno(" + QString::number(errno)
                                              + " ~ " + QTime::currentTime().toString() + ")"));
                return -1;
            }
           qDebug() << "TCPServer(): connection accepted %s " <<
                inet_ntoa(clientAddr.sin_addr); //change to emit

           writeToLog(log_, QString("\nIP: "
                                    + QString(inet_ntoa(clientAddr.sin_addr))
                                    + " (Connected: " + QTime::currentTime().toString()
                                    + ")"));

            //some sort of emit here inet_ntoa(clientAddr.sin_addr);
            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client[i] < 0) {
                    client[i] = newSocketDescriptor;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                qDebug() << "TCPServer(): Too many connections";
                writeToLog(errorLog_, QString("\nTCPServer(): Too many clients"
                                              + QTime::currentTime().toString()));
                return -1;
            }
            FD_SET((unsigned)newSocketDescriptor, &allset);

            if (newSocketDescriptor > maxfd) {
                maxfd = newSocketDescriptor;
            }
            if (i > maxi) {
                maxi = i;
            }
            if (--nready <= 0) {
                continue;
            }
        } //end add new socket
        //start check for new dat emit SignalClientConnected(inet_ntoa(clientAddr.sin_addr));a
        for (i = 0; i <= maxi; i++) {
            if ((recieveSocketDescriptor = client[i]) < 0) {
                continue;
            }
            if (FD_ISSET (recieveSocketDescriptor,&rset)) {
                tempMesg = mesg;
                bytesToRead = buflen_;
                while(bytesToRead != 0 && (n = recv(recieveSocketDescriptor, (char *)tempMesg, bytesToRead, 0)) > 0) {
                    tempMesg += n;
                    bytesToRead -= n;
                }

                //emit data to server probably have to copy this info for use in another thread
                qDebug() << mesg->ipAddr;
                qDebug() << mesg->data;
                qDebug() << QString::number(recieveSocketDescriptor).toLatin1().data();

                //emit packet that was recieved here

                if (n == 0) //connection closed by client
                {
                    writeToLog(log_, QString("\nIP: "
                                             + QString(inet_ntoa(clientAddr.sin_addr))
                                             + " (Disconnected: " + QTime::currentTime().toString()
                                             + ")"));
                    emit signalSocketClosed(recieveSocketDescriptor);
                    qDebug() << "TCPServer(): Connection disconnected %s" <<
                           inet_ntoa(clientAddr.sin_addr);
                    closesocket(recieveSocketDescriptor);
                    FD_CLR((unsigned)recieveSocketDescriptor, &allset);
                    client[i] = -1;
                }
                if (--nready <= 0) {
                    break;
                }
            } //end of FD_ISSET
        } //end of read/write loop
    } //end of while loop
}

int Socket::UDPServer() {
    //Packet p = new Packet();
    //need some sort of bind or accept i forget which
    while (true) {
        //if (rx(mesg) != -1) {
        //    emit signalPacketRecieved(p);
        //}

    }
    return 1;
}

int Socket::SetupSocket(const char * str) {
    struct hostent *hp;
    memset(&server_,0, sizeof(struct sockaddr_in));
    server_.sin_family = AF_INET;
    server_.sin_port = htons(sPort_);
    if (str != 0) {
        if ((hp = gethostbyname(str)) == NULL) {
            qDebug() << "SetupSocket(): getHostByName(): No such server available.";
            writeToLog(errorLog_, QString("\nSetupSocket(): getHostByName - Errno(" + QString::number(errno)
                                          + " ~ " + QTime::currentTime().toString() + ")"));
            return -1;
        }
        memcpy(hp->h_addr, (char *)&server_.sin_addr, hp->h_length);
    } else {
        server_.sin_addr.s_addr = htonl(INADDR_ANY);
        return 1;
    }
    client_.sin_family = AF_INET;
    client_.sin_port = htons(sPort_);

    return 1;
}

void Socket::SetAsClient(const char * str) {
    if (SetupSocket(str) == -1) {
        return;
    }
    switch(socketType_) {
    case UDP:
        if (bind(socketDescriptor_, (struct sockaddr *)&client_ ,
                 sizeof(client_)) == -1) {
            qDebug() << "SetAsClient(): failure to bind to port";
            return;
        }
        break;
    case TCP:
        if (::connect(socketDescriptor_, (struct sockaddr*)&server_,
                      sizeof(server_)) == -1) {
            qDebug() << "SetAsClient(): failure to connect to port";
            qDebug() << QString::number(errno).toLatin1().data();
            writeToLog(errorLog_, QString("\nSetAsClient(): connect - Errno(" + QString::number(errno)
                                          + " ~ " + QTime::currentTime().toString() + ")"));
            return;
        }
        break;
    }

    return;
}

void Socket::createTCPSocket() {
    int arg = 1;
    if ((socketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            qDebug() << "createTCPSocket(): Cannot Create Socket";
            writeToLog(errorLog_, QString("\ncreateTCPSocket(): socket - Errno(" + QString::number(errno)
                                          + " ~ " + QTime::currentTime().toString() + ")"));
    }
    if (setsockopt (socketDescriptor_, SOL_SOCKET, SO_REUSEADDR, (const char *) &arg,
                    sizeof(arg)) == -1) {
        writeToLog(errorLog_, QString("\nTCPServer(): setsockopt - Errno(" + QString::number(errno)
                                      + " ~ " + QTime::currentTime().toString() + ")"));
        qDebug() << "createTCPSocket(): setsockopt";
    }
    qDebug() << "createTCPSocket(): Socket Created";
    return;
}

void Socket::createUDPSocket() {
    int arg = 1;

    if ((socketDescriptor_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        qDebug() << "createUDPSocket(): Cannot Create Socket!" ;
    }

    if (setsockopt (socketDescriptor_, SOL_SOCKET, SO_REUSEADDR, (const char *)&arg,
                    sizeof(arg)) == -1) {
        qDebug() << "createUDPSocket(): setsockopt";
    }
}

char * Socket::createBuffer(Packet * pckt) {
    char * buffer = new char[pckt->length + LENGTHSIZE];
    memcpy(buffer,&(pckt->length), LENGTHSIZE);
    buffer[LENGTHSIZE - 1] = pckt->length;
    memcpy(&buffer[LENGTHSIZE], pckt->data,pckt->length);
    return buffer;
}


int Socket::tx(Packet * pckt) {
    int temp;
    char * buffer = createBuffer(pckt);
    switch (socketType_) {
    case TCP:
        qDebug() << QString::number(socketDescriptor_).toLatin1().data();
        if((temp = send(socketDescriptor_, buffer, pckt->length, 0)) == -1) {
            writeToLog(errorLog_, QString("\ntx(): send - Errno(" + QString::number(errno)
                                          + " ~ " + QTime::currentTime().toString() + ")"));
        }
        return -1;
    case UDP:
        return sendto(socketDescriptor_, buffer, pckt->length, 0,
                      (struct sockaddr*)&server_, serverLength_);
    default:
        qDebug() << "Socket(): invalid socket type";
        return -1;
    }
    return temp;
}

int Socket::tx(Packet * pckt, int SocketID) {
    //used only for TCP packets
    //probably need mutex to assess the socket list
    int temp;
    char * buffer;
    //check to see if socketID has not disconnect
    buffer = createBuffer(pckt);
    if((temp = send(SocketID, buffer, pckt->length, 0)) == -1) {
        writeToLog(errorLog_, QString("\ntx(): send - Errno(" + QString::number(errno)
                                      + " ~ " + QTime::currentTime().toString() + ")"));
    }
    return temp;
}

int Socket::tx(Packet *pckt, char *ipAddr) {
    //this is for udp transmissions
    return 0;
}

int Socket::rx(MessageStruct * mesg) {
    int n = 0;
    int bytesToRead = buflen_;
    int length = sizeof(server_);
    while((n != buflen_)) {
        switch (socketType_) {
        case TCP:
            n = recv(socketDescriptor_, (char *) mesg, bytesToRead, 0);
            qDebug() << mesg->data;
            if (n == -1) {
                qDebug() << "Rx(): recv(): error";
                writeToLog(errorLog_, QString("\nrx(): recv - Errno(" + QString::number(errno)
                                              + " ~ " + QTime::currentTime().toString() + ")"));
                return -1;
            }
            if (n == 0) {
                return 0;
            }
            break;
        case UDP:
            //this line needs to be fixed
            n = recvfrom(socketDescriptor_, (char *) mesg, bytesToRead, 0, (struct sockaddr *) &server_,(socklen_t*) &length);
            qDebug() << mesg->data;
            if (n == -1) {
                qDebug() << "Rx(): recv(): error";
                writeToLog(errorLog_, QString("\nrx(): recv - Errno(" + QString::number(errno)
                                              + " ~ " + QTime::currentTime().toString() + ")"));
                return -1;
            }
            if (n == 0) {
                return 0;
            }
            break;
        default:
            qDebug() << "Rx(): invalid socket type";
            return -1;
        }
        bytesToRead -= n;
        mesg += n;
    }
    return n;
}

void Socket::closeSocket() {
    qDebug() << "Closing socket";
    qDebug() << QString::number(closesocket(socketDescriptor_)).toLatin1().data();
}

void Socket::writeToLog(QFile *log, QString logMesg) {
    log->open(QIODevice::Append);
    log->write(logMesg.toLatin1().data());
    log->close();
}
