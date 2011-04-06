#include <iostream>
#include "server.h"
#include <QFile>
#include <QDebug>

#include "thread.h"
#include "serverfiletransfer.h"
#include "componentvoice.h"

Server::Server(int port, int backlog) : socket_(new Socket(kTCP)), running_(false) {

    socket_->bind(port, htonl(INADDR_ANY));
    socket_->listen(backlog);
}

Server::~Server() {
    // prepare thread for close by aborting socket operations and stopping the loop from repeating.
    running_ = false;
    delete socket_;

    // wait for the thread to finish.
    QThread::terminate();

    // destroy all sockets in the server.
    foreach (Socket* socket, clients_) {
        delete socket;
    }

    QThread::wait();
}

void Server::run() {
    int numReady;
    int largest = *socket_;
    int connected;
    socklen_t infoSize;
    sockaddr_in info;
    fd_set rset;
    fd_set allset;

    FD_ZERO(&allset);
    FD_SET((unsigned)*socket_, &allset);

    running_ = true;

    while (running_) {
        rset = allset; // check all currenet sockets
        numReady = select(largest + 1, &rset, NULL, NULL, NULL);

        // check if return was from server shutting down and close all current
        // connections and return if it was.
        if (running_ == false) {
            for (int i = 0; i < clients_.size(); i++) {
                delete clients_[i];
            }
            clients_.clear();
            return;
        }

        // check for a new connection
        if (FD_ISSET(*socket_, &rset)) {
            infoSize = sizeof(info);

            if ((connected = accept(*socket_, (sockaddr*) &info, &infoSize)) == -1) {
                //qDebug() << "accept error:" << strerror(errno);
                return; // TODO: inform main window of failure.
            } else {
                qDebug() << "Client connected.";
                clients_.append(new Socket(connected, kTCP, info));
                emit signalClientConnect(clients_.last());
            }

            if (clients_.size() == FD_SETSIZE) {
                // TODO: Handle too many connections (low priority)
            }

            FD_SET((unsigned)connected, &allset);      // add new descriptor to set

            // ensure the largest descriptor is still used.
            largest = connected > largest ? connected : largest;


            if (--numReady <= 0) {
                continue;	// no more readable descriptors
            }
        }

        // check the connected sockets for activity
        for (int i = 0; i < clients_.size(); i++) {
            if (FD_ISSET(*clients_[i], &rset)) {
                int msgSize;
                // If there is no data from the client, it disconnected.
                if (clients_[i]->receive((char*) &msgSize, sizeof(int)) <= 0) {
                    delete clients_[i];
                    clients_.remove(i);
                    continue;
                }
                char* buffer = new char[msgSize];
                clients_[i]->receive(buffer, msgSize);

                processClientMessage(clients_[i], buffer, msgSize);
                // check what the received data is for and send it to that component (through a signal)

                if (--numReady == 0) {
                    break;
                }
            }
        }
    }
}

void Server::processClientMessage(Socket *clientSocket, char *buffer, int msgSize) {

    int type;
    memcpy(&type, buffer, sizeof(int));
    switch (type) {
    case kVoice:
        serverVoiceComponent(clientSocket, buffer, msgSize);
        break;
    case kTransfer:
        startFileTransfer(QString(buffer + sizeof(int)), clientSocket);
        break;
    case kFileList:
        break;
    case kStream:
        break;
    default:
        delete[] buffer;
        break;
    }
}

void Server::slotDisconnectStream() {
    foreach (Socket* client, clients_) {
        Packet packet;
        packet.length = 0;
        packet.type = kStream;

        client->transmit(packet);
    }
}

void Server::slotPlayThisSong(QString songname) {
    QString title = songname.split('/').last();
    foreach (Socket* client, clients_) {
        Packet packet;
        packet.length = title.size() + 1;
        packet.type = kStream;
        packet.data = (char*) title.toStdString().c_str();
        client->transmit(packet);
    }
}

void Server::startFileTransfer(QString fileName, Socket * s) {
    Thread *thread = new Thread();
    ServerFileTransfer *sft = new ServerFileTransfer(fileName, s);
    connect(this, SIGNAL(signalStreamFile()), sft, SLOT(slotStartTransfer()));
    thread->start();
    sft->moveToThread(thread);
    emit signalStreamFile();
}

void Server::setupVoiceComponent(Socket * socket) {
    Thread *thread = new Thread();
    ComponentVoice *cv = 0;
    try {
        cv = new ComponentVoice(socket);
    } catch (const QString& e) {
        qDebug() << e;
        thread->terminate();
        delete thread;
        delete cv;
        return;
    }
    thread->start();
    cv->moveToThread(thread);
    QObject::connect(this, SIGNAL(signalStopVoiceComponent()),cv, SLOT(slotStopVoiceComponent()));
    QObject::connect(this,SIGNAL(serverVoiceMessage(char*,int)), cv,SLOT(receiveData(char*,int)));
    QObject::connect(this,SIGNAL(signalStartComponentVoice()),cv,SLOT(slotStartComponentVoice()));
    emit signalStartComponentVoice();
    return;
}

void Server::serverVoiceComponent(Socket * socket, char * buffer, int length) {
    if (length == sizeof(int) + 1) {
        if (buffer[sizeof(int)] == 1) {
            setupVoiceComponent(socket);
            return;
        }
        if (buffer[sizeof(int)] == 0) {
            qDebug() << "Deleting microphone on server";
            //emit signalStopVoiceComponent();
            return;
        }
    }
    emit serverVoiceMessage(buffer,length);
}

