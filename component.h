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
    virtual ~Component();

protected:
    /**
      Threaded action,
      Must be implemented by component extending this.

      @author Nick Huber
      */
    virtual void run() = 0;

protected:
    Socket* socket_; /**< the socket for future transmissions on this object */
    bool running_;   /**< Used in run loops to determine if the component should be running */
};

#endif // COMPONENT_H
