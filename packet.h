#ifndef PACKET_H
#define PACKET_H

#include "component.h"

struct Packet {
    int length; //equal to size of componetID + data;
    ComponentType type;
    char* data;
};

//struct Message {
//    int socketID;
//    char idAddr[16];
//    Packet * payload;
//};

#endif // PACKET_H
