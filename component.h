#ifndef COMPONENT_H
#define COMPONENT_H

#include <QThread>

#include "socket.h"

enum ComponentType {
    kStream,    /**< Multicast Music streaming. */
    kTransfer,  /**< File transfer. */
    kVoice,     /**< Server-Client Voice Chat. */
};

class Component : public QThread {

public:
    Component(Socket* socket);
    virtual ~Component() {}

protected:
    virtual void run() = 0;

protected:
    Socket* socket_;
    bool running_;

};

#endif // COMPONENT_H
