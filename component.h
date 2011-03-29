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
      Newly received data to be processed by the component.
      Must be implemented by each component.

      @param data Received data.
      @param length Length of received data.
      @author Nick Huber

      */
    virtual void receiveData(char* data, int length) = 0;

    /**
      Transmit data to the other side.


      @param data Data to transmit.
      @param length Size of data.
      @author Nick Huber
      */
    void transmitData(char* data, int length) { socket_.transmit(data, length); };

private:
    ComponentType type_;
    Socket socket_;

};

#endif // COMPONENT_H
