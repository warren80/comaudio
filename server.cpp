#include <iostream>
#include <QVector>
#include "server.h"
#include <QFile>
#include <QDebug>

#include "thread.h"
#include "serverfiletransfer.h"

Server::Server(int port, int backlog) : socket_(new Socket(kTCP)), running_(false) {

    socket_->bind(port);
    socket_->listen(backlog);
}

Server::~Server() {
    // prepare thread for close by aborting socket operations and stopping the loop from repeating.
    running_ = false;
    delete socket_;

    // wait for the thread to finish.
    QThread::wait();
}

void Server::run() {
    int numReady;
    int largest = *socket_;
    int connected;
    QVector<Socket*> clients;
    socklen_t infoSize;
    sockaddr_in info;
    fd_set rset;
    fd_set allset;

    FD_ZERO(&allset);
    FD_SET(*socket_, &allset);

    running_ = true;

    running_ = true;

    while (running_) {
        rset = allset; // check all currenet sockets
        numReady = select(largest + 1, &rset, NULL, NULL, NULL);

        // check if return was from server shutting down and close all current
        // connections and return if it was.
        if (running_ == false) {
            for (int i = 0; i < clients.size(); i++) {
                delete clients[i];
            }
            clients.clear();
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
                clients.append(new Socket(connected, kTCP, info));
            }

            if (clients.size() == FD_SETSIZE) {
                // TODO: Handle too many connections (low priority)
            }

            FD_SET(connected, &allset);      // add new descriptor to set

            // ensure the largest descriptor is still used.
            largest = connected > largest ? connected : largest;


            if (--numReady <= 0) {
                continue;	// no more readable descriptors
            }
        }

        // check the connected sockets for activity
        for (int i = 0; i < clients.size(); i++) {
            if (FD_ISSET(*clients[i], &rset)) {
                int msgSize;
                // If there is no data from the client, it disconnected.
                if (clients[i]->receive((char*) &msgSize, sizeof(int)) <= 0) {
                    delete clients[i];
                    clients.remove(i);
                    continue;
                }
                char* buffer = new char[msgSize];
                clients[i]->receive(buffer, msgSize);

                // check what the received data is for and send it to that component (through a signal)

                if (--numReady == 0) {
                    break;
                }
            }
        }
    }
}

void Server::startFileTransfer(QString fileName, Socket * s) {
    Thread *thread = new Thread();
    serverFileTransfer *sft = new serverFileTransfer(fileName, s);
    connect(this, SIGNAL(signalStreamFile()), sft, SLOT(slotStartTransfer()));
    connect(sft, SIGNAL(signalTransferDone()), thread, SLOT(deleteLater()));
    thread->start();
    emit signalStreamFile();
}
