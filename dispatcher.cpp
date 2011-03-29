#include "dispatcher.h"
#include "packet.h"

void Dispatcher::dispatch(char* data, int length) {
    Packet* packet = new Packet;
    packet->length = length;
    // copy the data into the rest of the packet then delete the original data.
    memcpy((void*) (packet + sizeof(packet->length)), data, length);
    delete[] data;

    // check if this connection already has this component
    // if it does, send it the data
    // if it does not, create it and send it the data

}
