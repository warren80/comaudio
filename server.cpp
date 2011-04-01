#include <iostream>
#include <QVector>
#include "server.h"
#include <QFile>
#include <QDebug>
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
                clients.append(new Socket(connected, kTCP, info));
            }

            if (clients.size() == FD_SETSIZE) {
                // TODO: Handle too many connections (low priority)
            }

            FD_SET(connected, &allset);      // add new descriptor to set

            // ensure the largest descriptor is still used.
            largest = connected > largest ? connected : largest;

            // TEMPORARY TEST
            qDebug() << "loaded client" << connected;
                QFile temp("/Volumes/OptiBay/Home/Dropbox/School/sem4/COMP4985/final/1.wav");
            temp.open(QFile::ReadOnly);
            qDebug() << "file size: " << temp.size();
            while (!temp.atEnd()) {
                char* readBuffer = new char[61440];
                int read = temp.read(readBuffer, 61440);
                clients.last()->transmit((char*) &read, sizeof(read));
                clients.last()->transmit(readBuffer, read);
            }
            qDebug() << "finished transmit";


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

                // move the received data to the dispatcher
                dispatcher_.dispatch(*clients[i], buffer, msgSize);

                if (--numReady == 0) {
                    break;
                }
            }
        }
    }
}
