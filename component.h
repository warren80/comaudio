#ifndef COMPONENT_H
#define COMPONENT_H

#include "socket.h"

enum ComponentType {
    kChat,      /**< Textual chat. */
    kStream,    /**< Music streaming. */
    kTransfer,  /**< File transfer. */
    kVoice,     /**< Multicast voice chat. */
};

class Component {

public:
    Component(ComponentType type, Socket socket);
    virtual ~Component() {};

    /**
      New data to be processed by the component.
      Must be implemented by each component.

      @param data Received data.
      @param length Length of received data
      @author Nick Huber

      */
    virtual void newData(char* data, int length) = 0;

private:
    ComponentType type_;
    Socket socket_;

};

#endif // COMPONENT_H
