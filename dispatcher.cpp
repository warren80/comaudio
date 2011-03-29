#include <Qdebug>

#include "dispatcher.h"
#include "packet.h"

void Dispatcher::dispatch(char* data, int length) {
    Packet* packet = new Packet;
    packet->length = length;
    // copy the data into the rest of the packet then delete the original data.
    memcpy((void*) (packet + sizeof(packet->length)), data, length);
    delete[] data;

    // testing, print out the type
    qDebug() << packet->type;
}
