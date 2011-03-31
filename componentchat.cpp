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
    message.leftJustified(4);
    //tx...(mesg, length);
}
