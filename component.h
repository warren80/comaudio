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
    Component(const Socket& socket);
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

    /**
      Transmit data to the other side.

      @param data Data to transmit.
      @param length Size of data.
      @author Nick Huber
      */
    void transmitData(char* data, int length) { socket_.transmit(data, length); }

protected:

protected:
    Socket socket_;

public slots:
    /**
      Slot for transmission of data to this component's socket.

      @param data Data to transmit.
      @param length Length of data to transmit.
      @author Nick Huber
      */
    void slot_transmitData(char* data, int length) { transmitData(data, length); }
signals:
    void signal_receviedData(char* data, int length);

};

#endif // COMPONENT_H
