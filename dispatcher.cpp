#include "dispatcher.h"
#include "packet.h"
#include "componentstream.h"

#include <QDebug>
void Dispatcher::dispatch(const Socket& socket, char* data, int length) {

    if (components_.size() == 0) {
        components_.addComponent(new ComponentStream(socket));
        ((ComponentStream*) components_[0])->setupAudio(44100, 2, 16, 39379580);
    }

    components_[0]->receiveData(data, length);
    //    Packet* packet = new Packet;
//    packet->length = length;
//    // copy the data into the rest of the packet then delete the original data.
//    memcpy((void*) (packet + sizeof(packet->length)), data, length);
//    delete[] data;

    // check if this connection already has this component
    // if it does, send it the data
    // if it does not, create it and send it the data

}
