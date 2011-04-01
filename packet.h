#ifndef PACKET_H
#define PACKET_H

#include "componenttype.h"

struct Packet {
    int length; //equal to size of ComponentType + data;
    ComponentType type;
    char* data;
};

#endif // PACKET_H
