#ifndef COMPONENTCHAT_H
#define COMPONENTCHAT_H

#include <QString>

#include "component.h"

class ComponentChat : public Component {
public:
    ComponentChat(Socket socket);
    /**
     * Call this function to send the message the user typed into the
     * chat input box.
     *
     * @author Karl Castillo
     * @arg message - the message that the user typed into the chat
     *                input box
     */
    void sendMessage(QString message);
};

#endif // COMPONENTCHAT_H
