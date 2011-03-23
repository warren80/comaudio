#include "component.h"

/**
 * CONSTRUCTOR
 */
Component::Component() {
    errorLog_ = new Logs("./logs/errorLog.log", "Error Log");
    activityLog_ = new Logs("./logs/log.log", "Activity Log");
}

/**
 * DESTRUCTOR
 */
void Component::slotPacketReceived(void * pckt) {
}
