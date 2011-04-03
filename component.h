#ifndef COMPONENT_H
#define COMPONENT_H

#include <QThread>

#include "socket.h"

class Component : public QThread {

public:
    Component(Socket* socket);
    virtual ~Component() {
        if (socket_ != 0) {
            delete socket_;
        }
    }

protected:
    virtual void run() = 0;

protected:
    Socket* socket_;
    bool running_;

};

#endif // COMPONENT_H
