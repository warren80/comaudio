
#include <QVector>
#include "server.h"

Server::Server(int port) : socket_(Socket(kTCP)), running_(false) {

    socket_.bind(port);
    socket_.listen(5);
}

void Server::run() {
    int numReady;
    int largestDescriptor = socket_;
    int newClientSocket;
    QVector<Socket*> clients;
    socklen_t client_len;
    sockaddr_in client_addr;
    fd_set rset;
    fd_set allset;

    FD_ZERO(&allset);
    FD_SET(socket_, &allset);

    while (running_) {
        rset = allset;               // structure assignment
        numReady = select(largestDescriptor + 1, &rset, NULL, NULL, NULL);

        // check if return was from server shutting down and close all current
        // connections and return if it was.
        if (running_ == false) {
            for (int i = 0; i < clients.size(); i++) {
                delete clients[i];
            }
            clients.clear();
            return;
        }

        if (FD_ISSET(socket_, &rset)) {
            client_len = sizeof(client_addr);

            if ((newClientSocket = accept(socket_, (sockaddr*) &client_addr, &client_len)) == -1) {
                //qDebug() << "accept error:" << strerror(errno);
                return; // TODO: inform main window of failure.
            } else {
                clients.append(new Socket(newClientSocket, kTCP, client_addr));
            }

            if (clients.size() == FD_SETSIZE) {
                qDebug() << "Too many clients";
            }

            FD_SET(newClientSocket, &allset);      // add new descriptor to set

            // ensure the largest descriptor is still used.
            if (newClientSocket > largestDescriptor) {
                largestDescriptor = newClientSocket;
            }

            if (--numReady <= 0) {
                continue;	// no more readable descriptors
            }
        }

        // check the connected descriptors for data
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
