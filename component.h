#ifndef COMPONENT_H
#define COMPONENT_H

#include <QThread>

#include "socket.h"

class Component : public QThread {

public:
    /**
     * Stores the socket information upon creation of a component.
     * @author Warren Voelkl, Nick Huber
     */
    Component(Socket* socket);
    virtual ~Component() {
        if (socket_ != 0) {
            delete socket_;
        }
    }

protected:
    virtual void run() = 0;

protected:
    Socket* socket_; /**< the socket for future transmissions on this object */
    bool running_;   /**< Used in run loops to determine if the component should be running */
};

#endif // COMPONENT_H
