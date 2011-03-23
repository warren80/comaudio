#include "componenttext.h"

/**
 * CONSTRUCTOR
 */
ComponentText::ComponentText(int socketID) : Component(socketID) {

}

/**
 * ESSENTIAL METHODS
 */

/*
 * TODO:
 *  1. Send text
 */
void ComponentText::sendMessage(QString message) {
    char *mesg = message.toLatin1().data();
    int length = message.length();

    //tx...(mesg, length);
}

/**
 * SLOTS
 */

/*
 * TODO:
 *  1. Create sockets.
 */
void ComponentText::slotStart() {

}
