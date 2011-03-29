#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

#include "socket.h"

enum ComponentType {
    kChat,      /**< Textual chat. */
    kStream,    /**< Music streaming. */
    kTransfer,  /**< File transfer. */
    kVoice,     /**< Multicast voice chat. */
};

class Component : public QObject {

public:
    Component(ComponentType type, const Socket& socket);
    virtual ~Component() {};

    /**
      Newly received data to be processed by the component.
      Must be implemented by each component.
      Should emit the signal_receivedData(char*, int) signal.

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

public slots:
    void slot_transmitData(char* data, int length) { transmitData(data, length); };

signals:
    void signal_receviedData(char* data, int length);

};

#endif // COMPONENT_H
