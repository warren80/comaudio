#include "component.h"

/**
 * CONSTRUCTOR
 */
Component::Component(int socketID) {
    errorLog_ = new Logs("./logs/errorLog.log", "Error Log");
    activityLog_ = new Logs("./logs/log.log", "Activity Log");
    socketID_ = socketID;
}

/**
 * DESTRUCTOR
 */
void Component::slotPacketReceived(void * pckt) {
}

void Component::slotStart(){

}
