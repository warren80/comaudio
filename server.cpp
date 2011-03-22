#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
}

void Server::slotMessageFromSocket(void * buf) {
    emit signalDispatcher(void *);
}
