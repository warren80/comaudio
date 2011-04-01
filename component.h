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

    /**
      Newly received data to be processed by the component.
      Must be implemented by each component.
      Should emit the signal_receivedData(char*, int) signal.

      @param data Received data.
      @param length Length of received data.
      @author Nick Huber

      */
    virtual void receiveData(char* data, int length) = 0;

protected:
    virtual void run() = 0;

protected:
    Socket* socket_;
    bool running_;

};

#endif // COMPONENT_H
