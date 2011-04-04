#ifndef PACKET_H
#define PACKET_H

#include "componenttype.h"

struct Packet {
    int length;         /**< equal to size of ComponentType + data */
    ComponentType type; /**< The component this message coresponds to */
    char* data;         /**< The data portion of the packet */
};

#endif // PACKET_H
