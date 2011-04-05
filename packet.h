#ifndef PACKET_H
#define PACKET_H

#include "componenttype.h"

/**
  Packet format to send data as.

  @author Nick Huber
  */
struct Packet {
    Packet() {
        length = 0;
        data = 0;
    }
    int length;         /**< equal to size of ComponentType + data */
    ComponentType type; /**< The component this message coresponds to */
    char* data;         /**< The data portion of the packet */
};

#endif // PACKET_H
