#include "componentchat.h"

/**
 * CONSTRUCTOR
 */
ComponentChat::ComponentChat(const Socket& socket) : Component(kChat, socket) {

}


/**
 * ESSENTIAL METHODS
 */

/*
 * TODO:
 *  1. Send text
 */
void ComponentChat::sendMessage(QString message) {
    char *msg = message.toLatin1().data();
    int length = message.length();

    //tx...(mesg, length);
}
