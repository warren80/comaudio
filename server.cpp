
#include <QVector>
#include "server.h"

Server::Server(int port, int backlog) : socket_(Socket(kTCP)), running_(false) {

    qDebug() << "Server starting...";
    socket_.bind(port);
    qDebug() << "Listening for clients.";
    socket_.listen(backlog);
}

void Server::run() {
    int numReady;
    int largest = socket_;
    int connected;
    QVector<Socket*> clients;
    socklen_t infoSize;
    sockaddr_in info;
    fd_set rset;
    fd_set allset;

    FD_ZERO(&allset);
    FD_SET(socket_, &allset);

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
        if (FD_ISSET(socket_, &rset)) {
            infoSize = sizeof(info);

            if ((connected = accept(socket_, (sockaddr*) &info, &infoSize)) == -1) {
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
                // temp. echo and delete received data.
                qDebug() << buffer;
                delete[] buffer;

                if (--numReady == 0) {
                    break;
                }
            }
        }
    }
}
