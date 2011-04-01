#include "component.h"

/**
 * CONSTRUCTOR
 */
Component::Component(int socketID) {
    socketID_ = socketID;
}

/**
 * DESTRUCTOR
 */
void Component::slotPacketReceived(void * pckt) {
}

void Component::slotStart(){

}
